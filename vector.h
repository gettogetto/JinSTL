#ifndef JIN_VECTOR_H_

#define JIN_VECTOR_H_
#include <stddef.h>
#include "iterator.h"
#include "alloc.h"
#include "type_traits.h"
#include "uninitialized.h"
#include "construct.h"
namespace jinstl{
	template<class T,class Alloc = alloc>
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
					size_type oldcapacity = capacity();
					size_type old_element_count = size();
					size_type new_size = old_element_count?2*old_element_count:1;
					try{
						iterator newstart = data_allocator::allocate(new_size);
					
						iterator insert_pos = uninitialized_copy(start,pos,newstart);
						construct(insert_pos,*pos);
						++insert_pos;
						iterator newfinish = uninitialized_copy(pos,finish,insert_pos);
					}catch(...){
						destroy(newstart,newfinish);
						data_allocator::deallocate(newstart,new_size);
						throw;
					}
					destroy(start,finish);
					deallocate();	
					start = newstart;
					finish = newfinish;
					end_of_storage = start + new_size; 
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
			vector():start(0),finish(0),end_of_storage(0){}	
			vector(size_type n,const T& val){
				fill_initialize(n,val);
			}
			vector(int n,const T& val){
				fill_initialized(n,val);
			}
			vector(long n,const T& val){
				fill_initialized(n,val);
			}
			explicit vector(size_type n){
				fill_initialized(n,val);
			}
			~vector(){
				destroy(start,finish);//a global function,call destruct function
				deallocate();
			}
			


			void insert(iterator pos,size_type n,const value_type& val){
				
			}
		};//namespace jinstl



#endif//JIN_VECTOR_H_
