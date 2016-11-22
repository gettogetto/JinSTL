#ifndef JIN_UNINITIALIZED_H
#define JIN_UNINITIALIZED_H
#include<string.h>
#include"construct.h"
#include"type_traits.h"
#include"iterator.h"
#include "algobase.h"
namespace jinstl{




	template<class 	InputIterator,class ForwardIterator>
		inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator res,__true_type){
			return copy(first,last,res);
		}	

	template<class InputIterator,class ForwardIterator>
		inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputIterator last,ForwardIterator res,__false_type){
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

	template<class InputIterator,class ForwardIterator,class T>
		inline  ForwardIterator __uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator res,T*){
			typedef typename __type_traits<T>::is_POD_type is_POD_type;
			return __uninitialized_copy_aux(first,last,res,is_POD_type());	
		}

	template<class InputIterator,class ForwardIterator>
		 inline ForwardIterator uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator res){
			return __uninitialized_copy(first,last,res,value_type(first));	

		}

//specialized char* and wchar_t*
	inline char *uninitialized_copy(const char *first,const char *last,char *res){
			memmove(res,first,last-first);
			return res+(last-first);
		}
	inline wchar_t *uninitialized_copy(const wchar_t *first,const wchar_t *last,wchar_t *res){
			memmove(res,first,(last-first)*sizeof(wchar_t));
			return res+(last-first);
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
template<class ForwardIterator,class T,class InputIterator>
inline ForwardInputIterator __uninitialized_fill_copy(ForwardItIterator res,ForwardItIterator mid,const T& x,InputIterator first,InputIterator last){
	uninitialized_fill(res,mid,x);
	try{
		return uninitialized_copy(first,last,mid);
	}catch{
		destroy(res,mid);
	}


}

template<class InputIterator,class ForwardIterator,class T,class T>
inline void __uninitialized_copy_fill(InputIterator first1,InputIterator last1,InputIterator first2,InputIterator last2,const T& x){
	ForwardIterator mid2 = uninitialized_copy(first1,last1,first2);
	try{
		uninitialized_fill(mid2,last2,x);
	}catch{
		destroy(first2,mid2);
	}


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
