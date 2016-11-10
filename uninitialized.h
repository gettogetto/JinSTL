#ifndef JIN_UNINITIALIZED_H
#define JIN_UNINITIALIZED_H
#include<string.h>
#include"construct.h"
#include"traits_type.h"
#include"iterator.h"
namespace jinstl{




	template<class 	InputIterator,class ForwardItetrator>
		inline ForwardItetrator __uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator res,__true_type){
			return copy(first,last,res);
		}	

	template<class InputIterator,class ForwardItetrator>
		inline ForwardItetrator __uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator res,__false_type){
			ForwardIterator cur = res;
			try{
				for(;first!=last;++first,++cur){
					construct(&*cur,*first);
				}
				return cur;
			}catch(...){
				destroy(res,cur);
				throw;
			}	
		}	

	template<class InputIterator,class ForwardItetrator,class T,class T1>
		inline  ForwardItetrator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator res,T1*){
			typedef typename __traits_type<T>::is_POD_type is_POD_type;
			__uninitializeds_copy_aux(first,last,res,is_POD_type());	
		}

	template<class InputIterator,class ForwardItetrator>
		 inline ForwardItetrator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator res){
			return __uninitializeds_copy(first,last,res,value_type(first));	

		}


	inline char *uninitialized_copy(const char *first,const char *last,char *res){
			memmove(res,first,last-first);
			return res+last-first;
		}
	inline wchar_t *uninitialized_copy(const wchar_t *first,const wchar_t *last,wchar_t *res){
			memmove(res,first,(last-first)*sizeof(wchar_t));
			return res+last-first;
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<class ForwardIterator,class T>
		inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& val,__true_type){
			fill(first,last,val);
		}
	template<class ForwardIterator,class T>
		inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,const T& val,__false_type){
			ForwardIterator cur = first;
			try{
				for(;cur!=last;++cur){
					construct(&*cur,val);
				}
			}catch(...){
				destroy(first,cur);
				throw;
			}
			
		}
	template<class ForwardIterator ,class T,class T1>
		inline void __uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& val,T1*){
			typedef typename __type_traits<T1>::is_POD_type is_POD_type;
			__uninitialized_fill_aux(first,last,val,is_POD_type());
		}
	template<class ForwardIterator,class T>
		inline void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& val){
			__uninitialized_fill(first,last,val,value_type(first));	
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//POD:trival ctor/dtor/copy/assignment
	template<class ForwardIterator,class Size,class T>
		inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& val,__true_type){
			return fill_n(first,n,val);
		}
//not POD
	template<class ForwardIterator,class Size,class T>
		inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,const T& val,__false_type){
			//Size i = 0;
			ForwardIterator cur = first;
			try{	
				for(;n>0;--n,++cur){
					construct(&*cur,val);
				}
				return cur;
			}
//if exception,then destroy the constructed objs
			catch(...){
				destroy(first,cur);
				throw;
			}
		}
	template<class ForwardIterator,class Size,class T,class T1>
		inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& val,T1*){
			typedef typename __type_traits<T1>::is_POD_type is_POD_type;

			return __uninitialized_fill_n_aux(first,n,val,is_POD_type()); 
		}
	template<class ForwardIterator,class Size,class T>
		inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& val){

			return __uninitialized_fill_n(first,n,val,value_type(first)); 
		}
}

#endif//JIN_UNINITIALIZED_H
