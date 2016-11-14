#ifndef JIN_ALLOC_H_
#define JIN_ALLOC_H_
#include<cstdlib>

namespace jinstl{
	#ifndef __THROW_BAD_ALLOC
	#include<iostream>
        #define __THROW_BAD_ALLOC do{cerr<<"out of memory"<<endl;exit(1);}while(0)
	#endif


	#include<stddef.h>
	#include<stdlib.h>
	#include<string.h>
	#include<assert.h>
	template<int inst>
	class __first_alloc_template{
	private:
		static void *oom_malloc(size_t);
		static void *oom_realloc(void*,size_t);
		static void (*__malloc_alloc_handler)();
	public:
		static void *allocate(size_t n){
			void* res = malloc(n);
			if(res==0){
				res=oom_malloc(n);
			}
			return res;
		}
		static void *deallocate(void* pointer,size_t n){
			free(pointer);
		}
		static void *reallocate(void* pointer,size_t old_size,size_t new_size){
			void* res = realloc(pointer,new_size);
			return res;	
		}
		static void (*set_malloc_handler(void (*f)))(){
			void(*old)() = __malloc_alloc_andler;
			__malloc_alloc_handler = f;	
			return old;
		}
	}
	void (*__first_alloc_template<int inst>::__malloc_alloc_handler)() = 0;
	template<int inst>
	void* __first_alloc_template<int inst>::oom_malloc(size_t n){
		void *res;
		void (*handler)();
		for(;;){
			handler = __malloc_alloc_handler;
			if(handler==0) __THROW_BAD_ALLOC;
			(*handler)();
			res =malloc(n);	
			if(res) return res;
		}	
	}

	template<int inst>
	void* __first_alloc_template<int inst>::oom_realloc(void* pointer,size_t n){
		void *res;
		void (*handler)();
		for(;;){
			handler = __malloc_alloc_handler;
			if(handler==0) __THROW_BAD_ALLOC;
			(*handler)();
			res=realloc(pointer,n);
			if(res) return res;
		}		

	}



//////////////////////////////////////////////////////////////////////////////////////////
	template<int inst>
	class __second_alloc_template{
	private:
		static const m_align = 8;
		static const m_maxbytes = 128;
		static const m_freelists_num= 16;
		//16 freelists for 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128 bytes
		static listnode* volatile m_freelist[m_freelists_num];
		//round up to 8 times
		static size_t round_up(size_t bytes){
			size_t new_bytes = (bytes+m_align-1)&~(m_align-1);
			return new_bytes;	
		}
		static size_t freelist_index(size_t bytes){
			size_t block_number = (bytes+m_align-1)/m_align - 1;
			return block_number;	
		}
			
	private:
//union save space
		union listnode{
			union listnode* next;
			char client_data[1];
		
		}
		
		static void *refill(size_t n);
		static char *chunk_alloc(size_t size,int &nobjs);
		
		static char *start_free;//memory pool start
		static char *end_free;//memory pool end
		static size_t heap_size;
	public:
		static void *allocate(size_t n);
		static void *deallocate(void *pointer,size_t n);
		static void *reallocate(void *pointer,size_t old_size,size_t new_size);	
		
	}
typedef __second_alloc_template<0> alloc;


template<int inst>
char *__second_alloc_template<inst>::start_free = 0;

template<int inst>
char *__second_alloc_template<inst>::end_free = 0;

template<int inst>
size_t __second_alloc_template<inst>::heap_size = 0;

template<int inst>
__second_alloc_template<inst>::listnode * volatile __second_alloc_template<inst>::m_freelist[m_freelist_num]={0};

template<inst>
	void *__second_alloc_template<inst>::allocate(size_t n){
		if(n>(size_t)m_maxbytes){
			return __first_alloc_template<inst>::allocate(n);	
		}
		//find the suitable list
		listnode *volatile *suitable_list=m_freelist+freelist_index(n);
		listnode *res = *suitable_list;
		if(res == 0){
			void *r = refill(round_up(n));
			return r;
		}
		*suitable_list = res->next;
		return res;
	}



template<inst>
	void __second_alloc_template<inst>::deallocate(void *pointer,size_t n){
		if(n>(size_t)m_maxbytes){
			__first_alloc_template<inst>::deallocate(pointer,n);
			return;	
		}
		listnode *volatile *suitable_list=m_freelist + freelist_index(n);
		((listnode*)pointer)->next=*suitable_list;
		*suitable_list = (listnode*)pointer;
	}
template<inst>
	void *__second_alloc_template<inst>::reallocate(void *pointer ,size_t oldsize,size_t newsize){
		deallocate(p,oldsize);
		void *res = allocate(newsize);
		return res;
	}
//get memory from pool to freelist
template<inst>
	void *__second_alloc_template<inst>::refill(size_t n){//n is 8(m_align) times bytes
		int nobjs = 20;
		//alloc nobjs blocks,every block is n bytes
		char *chunk = chunk_alloc(n,nobjs);
		listnode *volatile *suitable_list;
		listnode *res;
		listnode *cur_node,*next_node;
		int i;
		if(nobjs == 1) return chunk;
		suitable_list = m_freelist + freelist_index(n);
		res = (listnode *)chunk;//return for client
		//other nodes put in list
		*suitable_list = cur_node = next_node = (listnode *)(chunk+n);
		for(i=1;;i++){
			if(i==nobjs-1){
				cur_node->next = 0;
				break;
			}
			next_node = (char*)next_node+n;
			cur_node->next = next_node;
			cur_node = cur_node->next;		
		}
		return res;
	}
//get memory from pool 
template<inst>
	void *__second_alloc_template<inst>::chunk_alloc(size_t bytes,int& nobjs){
		size_t totalsize = bytes*nobjs;
		size_t bytesleft = end_free - start_free;
		void *res;
		if(bytesleft>=totalsize){
			res = start_free;
			start_free+=totalsize;
			return res;
		}else if(bytesleft>=bytes){//but enough for 1 or more obj
			
			res =start_free;
			nobjs = bytesleft/bytes;
			start_free += nobjs*bytes;
			return res;
		}else{//less than bytes
			size_t bytes_alloc = totalsize*2+round_up(heap_size>>4);
			//take the unuse-less-than-bytes-memory to freelist
			if(bytesleft>0){
				listnode *volatile *suitable_list=m_freelist+freelist_index(bytesleft);
				listnode* cur=*suitable_list;
				((listnode*)start_free)->next = cur;
				*suitable_list=(listnode*)start_free;
			}
			//alloc heap memory to give pool
			start_free=(char*)malloc(bytes_alloc);
			if(start_free==0){//malloc failed
				listnode *volatile *suitable_list,*p;
				int i;
				for(i=bytes;i<=m_maxbytes;i+=m_align){
					suitable_list = m_freelist + freelist_index(i);
					p=*suitable_list;
					if(p!=0){//get  other list's memory
						*suitable_list = p->next;
						start_free = (char*)p;
						end_free = start_free+i;
						return chunk_alloc(bytes,nobjs);//the i bytes is not sure used completely,recall chunk_alloc
					}
				}
				end_free=0;//all ways try but no enough memory
				//ask for the first_alloc_template
				start_free = __first_alloc_template<inst>::allocate(bytes_alloc);			
			}
			heap_size+=bytes_alloc;
			end_free = start_free+bytes_alloc;
			return chunk_alloc(bytes,nobjs);//now pool  has enough memory,recall chunk_alloc
			
		}
		

	}

	///////////////////////the interface for the first or second allocator//////////////////////////
	template<class T,class Alloc>
	class simple_alloc{
	public:
		static T *allocate(){
			return (T*)Alloc::allocate(sizeof(T));
		}
		static T *allocate(size_t n){
			return (n==0)?0:(T*)Alloc::allocate(sizeof(T)*n);
		}
		static void deallocate(void *pointer){
			Alloc::deallocate(pointer,sizeof(T));
		}
		static void deallocate(void *pointer,size_t n){
			if(n!=0) Alloc::deallocate(pointer,sizeof(T)*n);
		}
	}
}
#endif//JIN_ALLOC_H_
