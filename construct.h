#ifndef JIN_CONSTRUCT_H
#define JIN_CONSTRUCT_H


#include<new>//for placement new
#include"type_traits.h"
namespace jinstl{
	template <class T1,class T2>
		inline void construct(T1 *pointer,const T2& value){
			new(pointer) T1(value);
		}
	//destroy version 1
	template <class T>
		inline void destroy(T *pointer){
			pointer->~T();
		}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	template <class ForwardIterator>
		inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__false_type){
			while(first!=last){
				destroy(&*first);
				++first;
			}
		}

	template<class ForwardIterator>
		inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__true_type){

		}
	template <class ForwardIterator,class T>
		inline void __destroy(ForwardIterator first,ForwardIterator last,T*){
			typedef typename __type_traits<ForwardIterator>::has_trivial_destructor trivial_destructor;
			__destroy_aux(first,last,trivial_destructor());

		}
	//destroy version2
	template <class ForwardIterator>
		inline void destroy(ForwardIterator first ,ForwardIterator last){
			__destroy(first,last,value_type(first));

		}


	inline void destroy(char*,char*){}
	inline void destroy(wchar_t*,wchar_t*){}
}
#endif //JIN_CONSTRUCT_H
