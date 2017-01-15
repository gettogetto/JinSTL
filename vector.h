#ifndef JIN_VECTOR_H_

#define JIN_VECTOR_H_
#include <stddef.h>
#include "iterator.h"
#include "alloc.h"
#include "type_traits.h"
#include "uninitialized.h"
#include "construct.h"
#include<initializer_list>
namespace jinstl{
	template<class T,class Alloc = alloc>//__second_alloc_template<0>
		class vector{
		public:	
			//vector iterator is T*
			typedef T* iterator;
			typedef const T* const_iterator;
			typedef T* pointer;
			typedef T& reference;
			typedef ptrdiff_t difference_type;
			typedef T value_type;
			typedef size_t size_type;
		private:
			typedef simple_alloc<T,Alloc> data_allocator;
			iterator start;
			iterator finish;
			iterator end_of_storage;
		protected:	
			void deallocate(){
				if(start){
					data_allocator::deallocate(start,end_of_storage-start);
				}
			}
			iterator allocate_and_fill(size_type n,const T& val){
				iterator res = data_allocator::allocate(n);
				try{
					uninitialized_fill_n(res,n,val);
					return res;
				}catch(...){
					data_allocator::deallocate(res,n);
					throw;
				}
			}

			
			iterator allocate_and_copy(size_type n,const_iterator first,const_iterator last){
				iterator res = data_allocator::allocate(n);
				try{
					uninitialized_copy(first,last,res);
					return res;
				}catch(...){
					data_allocator::deallocate(res,n);
					throw;
				}
			}
			void fill_initialize(size_type n,const T& val){
				start=allocate_and_fill(n,val);
				finish = start + n;
				end_of_storage = finish;
			}	
			void insert_aux(iterator pos,const T& val){
				if(finish!=end_of_storage){//has space
					construct(finish,*(finish-1));
					++finish;
					copy_backward(pos,finish-2,finish-1);
					*pos = val;
				}else{
					
					size_type old_element_count = size();
					size_type new_capacity = old_element_count?2*old_element_count:1;
					iterator newstart = data_allocator::allocate(new_capacity);
					//every step should record the newfinish,if exception happend,destroy the constructed objs
					iterator newfinish = newstart;
					try{
						newfinish = uninitialized_copy(start,pos,newstart);
						construct(newfinish,val);
						++newfinish;
						newfinish = uninitialized_copy(pos,finish,newfinish);
					}catch(...){
						destroy(newstart,newfinish);
						data_allocator::deallocate(newstart,new_capacity);
						throw;
					}
					destroy(start,finish);
					deallocate();	
					start = newstart;
					finish = newfinish;
					end_of_storage = start + new_capacity; 
				}	
			}
		public:
			iterator begin() const {
				return start;
			}	
			iterator end() const {
				return finish;
			}
			reference front(){
				return *start;
			}
			reference back(){
				return *(finish-1);
			}
			size_type size() const {
				return finish - start;
			}
			size_type capacity() const {
				return end_of_storage - start;
			}
			bool empty() const{
				return start == finish;
			}
			reference operator[](size_type n){
				return *(start+n);
			}

			/*****************************************/	
			vector():start(0),finish(0),end_of_storage(0){}	
			//vector(std::initializer_list<T> list){
				//reserve(list.size());
				//insert(begin(),list.begin(),list.end());
			//}			
			vector(size_type n,const T& val){
				fill_initialize(n,val);
			}
			vector(int n,const T& val){
				fill_initialize(n,val);
			}
			vector(long n,const T& val){
				fill_initialize(n,val);
			}
			explicit vector(size_type n){
				fill_initialize(n,T());
			}
			vector(const_iterator first,const_iterator last){
				size_type n=0;
				n=distance(first,last);
				start = allocate_and_copy(n,first,last);
				finish = start+n;
				end_of_storage=finish;
			}
			~vector(){
				destroy(start,finish);//a global function,call destruct function
				deallocate();
			}
			/******************************************/
			void resize(size_type newsize,const T& val){
				if(newsize<size()){
					
					erase(start+newsize,finish);
				}
				else{
				
                                        insert(finish,newsize-size(),val);//fixed: change start to finish
				}
	
			}
			void resize(size_type newsize){
				resize(newsize,T());
			
			}

			//must copy to a new memory firstly,and then deallocate the old memory.
			void shrink_to_fit(){
				//if(finish==end_of_storage) return;
				//data_allocator::deallocate(finish,end_of_storage-finish);
				//end_of_storage = finish;
				//method above is wrong!!!
				T* new_start = static_cast<T*>(data_allocator::allocate(size()));
				T* new_finish = static_cast<T*>(uninitialized_copy(start,finish,new_start));
				data_allocator::deallocate(start,capacity());
				start = new_start;
				finish = new_finish;
				end_of_storage = finish;
				
			}
			void reserve(size_type n){
				if(n<capacity()) return;
				size_type oldsize = size();
				iterator newstart = allocate_and_copy(n,start,finish);
				destroy(start,finish);
				deallocate();
				start=newstart;
				finish=start+oldsize;
				end_of_storage=start + n;
			}
			/******************************************/
			void push_back(const T& val){
				if(finish!=end_of_storage){
					construct(finish++,val);
				}else{
                                        insert_aux(end(),val);
				}
			}
			void pop_back(){
				destroy(--finish);
			}
			/******************************************/
			iterator insert(iterator pos,const T& val){
				size_type n = pos-begin();
				if(pos==finish&&finish!=end_of_storage){
					construct(finish,val);
					++finish;
				}else{
					insert_aux(pos,val);
				}
				return begin()+n;
				
			}
			void insert(iterator pos,size_type n,const value_type& val){
				if(n==0) return; 
				if(n <= end_of_storage-finish){
					
                                        size_type count = finish - pos;
					iterator old_finish = finish;
                                        if(count>=n){
						uninitialized_copy(finish-n,finish,finish);
						finish += n;
						copy_backward(pos,old_finish-n,old_finish);
						fill(pos,pos+n,val);
					}else{
                                                uninitialized_fill_n(finish,n-count,val);
						finish+=n-count;
						uninitialized_copy(pos,old_finish,finish);
						finish +=count;
						fill(pos,old_finish,val);
					}
					
				}else{
					size_type old_capacity = capacity();
                                        size_type new_capacity = old_capacity+max(n,old_capacity);
					iterator newstart = data_allocator::allocate(new_capacity);
					iterator newfinish = newstart;
				
					//every step should record the newfinish,if exception happend,destroy the constructed objs
					try{
						newfinish  = uninitialized_copy(start,pos,newstart);
						newfinish = uninitialized_fill_n(newfinish,n,val);
                                                newfinish = uninitialized_copy(pos,finish,newfinish);//fixed:change newfinish+n to newfinish
					}catch(...){
						destroy(newstart,newfinish);
                                                data_allocator::deallocate(newstart,new_capacity);
						throw;
					}
					destroy(start,finish);
					deallocate();
					start = newstart;
					finish = newfinish;
					end_of_storage = newstart + new_capacity;
				}
			}
			void insert(iterator pos,const_iterator first,const_iterator last){
				size_type n;
				n=distance(first,last);
				if(n==0) return;
                                if(n <= end_of_storage-finish){
					
                                        size_type count = finish - pos;
					iterator old_finish = finish;
					if(count>n){
						uninitialized_copy(finish-n,finish,finish);
						finish += n;
						copy_backward(pos,old_finish-n,old_finish);
						copy(first,last,pos);
					}else{
                                                finish+=n;
                                                uninitialized_copy(pos,old_finish,pos+n);
                                                copy(first,last,pos);
                                                //uninitialized_copy(first+count,last,finish);
                                                //finish+=n-count;
                                                //uninitialized_copy(pos,old_finish,finish);
                                                //finish+=count;
                                                //copy(first,first+count,pos);
					}
					
				}else{
                                        size_type old_capacity = capacity();
                                        size_type new_capacity = old_capacity+(old_capacity>n?old_capacity:n);
					iterator newstart = data_allocator::allocate(new_capacity);
					iterator newfinish = newstart;
				
					//every step should record the newfinish,if exception happend,destroy the constructed objs
					try{
						newfinish  = uninitialized_copy(start,pos,newstart);
						newfinish = uninitialized_copy(first,last,newfinish);
						newfinish = uninitialized_copy(pos,finish,newfinish);
					}catch(...){
						destroy(newstart,newfinish);
						data_allocator::deallocate(newstart,new_capacity);
						throw;
					}
					destroy(start,finish);
					deallocate();
					start = newstart;
					finish = newfinish;
					end_of_storage = newstart + new_capacity;
				}
				
			}
			/******************************************/
			iterator erase(iterator pos){
				if(pos!=finish){
					copy(pos+1,finish,pos);
				}
				destroy(--finish);
				
				return pos;
			}
			iterator erase(iterator first,iterator last){
				
				iterator tmp = copy(last,finish,first);
				destroy(tmp,finish);
				finish -= last - first;
				return first;
			}
			void clear(){
				destroy(start,finish);
				finish =start;
				
			}
			/**************************************************/
			void swap(vector<T,Alloc>&v){
				swap(start,v.begin());
				swap(finish,v.end());
				swap(end_of_storage,v.end_of_storage);
			}
		};	
		template<class T,class Alloc>
		inline bool operator==(const vector<T,Alloc>& v1,const vector<T,Alloc>&v2){
			return v1.size()==v2.size() && equal(v1.begin(),v1.end(),v2.begin());
		}		
		template<class T,class Alloc>
		inline bool operator<(const vector<T,Alloc>&v1,const vector<T,Alloc>&v2){
			return lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end());
		}
		//template<class T,class Alloc>
		//inline bool operator>()(const vector<T,Alloc>&v1,const vector<T,Alloc>&v2){
			  
		//	return !lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end());
		//}
		
		template<class T,class Alloc>
		inline void swap(vector<T,Alloc>& v1,vector<T,Alloc>& v2){
			v1.swap(v2);
		}
};//namespace jinstl


#endif//JIN_VECTOR_H_
