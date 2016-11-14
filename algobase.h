#ifndef JIN_STL_ALGOBASE_H_
#define JIN_STL_ALGOBASE_H_



#include <string.h>

#include "iterator.h"
#include "pair.h"
#include "type_traits.h"

namespace jinstl {

	/*********************************************************************************/
	// copy
	//  [first, last) to [result, result + (last - first))内
	/*********************************************************************************/

	template <class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	// const char* 
	inline char* copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	// const wchar_t* 
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}
/*********************************************************************************/
	//  __copy_dispatch 
	template <class InputIterator, class OutputIterator>
	struct __copy_dispatch {
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
			return __copy(first, last, result, iterator_category(first));
		}
	};

	//  __copy_dispatch T* 
	template <class T>
	struct __copy_dispatch<T*, T*> {
		T* operator()(T* first, T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator Trivial;
			return __copy_t(first, last, result, Trivial());
		}
	};

	// __copy_dispatch const T* , T* 
	template <class T>
	struct __copy_dispatch<const T*, T*> {
		T* operator()(const T* first, const T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator Trivial;
			return __copy_t(first, last, result, Trivial());
		}
	};
/*********************************************************************************/
	// __copy  input_iterator_tag 
	template <class InputIterator, class OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag) {
		for (; first != last; ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	// __copy  random_access_iterator_tag 
	template <class RandomAccessIterator, class OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, random_access_iterator_tag) {
		return __copy_d(first, last, result, distance_type(first));
	}
/*********************************************************************************/
	// __copy_d :  n 
	template <class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*) {
		for (Distance n = last - first; n > 0; --n, ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	// __copy_t :  trivial assignment operator
	template <class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	// __copy_t :  non-trivial assignment operator
	template <class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
		// RandomAccessIterator
		return __copy_d(first, last, result, static_cast<ptrdiff_t*>(0));
	}



/*********************************************************************************/

	// copy_backward
	//  [first, last) to [result - (last - first), result)

	template <class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result) {
		return __copy_backward_dispatch<BidirectionalIterator1,BidirectionalIterator2>()(first,last,result);
	}

	//  __copy_backward_dispatch 
	template <class BidirectionalIterator1, class BidirectionalIterator2>
	struct __copy_backward_dispatch {
			BidirectionalIterator2 operator()(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 res){
				return __copy_backward(first,last,res,iterator_category(first),Distance*);
			}
	};
//biderational_iterator_tag
	template<class BidirectionalIterator1,class BidirectionalIterator2,class Distance>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 res,bidirectional_iterator_tag,Distance*){
		for(--first,--last;first!=last;--first,--last){
			*res = *last; 
		}
		return res;
	}
//random_iterator_tag
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 res,random_access_iterator_tag,Distance*){
		Distance n = last - first;
		for(;n>0;n--){
			*--res=*--last;
		}
		return res;
	}
	// __copy_backward_dispatch, T*
	template <class T>
	struct __copy_backward_dispatch<T*, T*> {
		T* operator()(T *first,T *last,T *res){
			return __copy_backward_t(first,last,res,typename __type_traits<T>::has_trivial_assignment_operator());
		}
	};

	//  __copy_backward_dispatch  const T*，T*
	template <class T>
	struct __copy_backward_dispatch<const T*, T*> {
		T* operator()(const T *first,const T *last,T *res){
			return __copy_backward_t(first,last,res,typename __type_traits<T>::has_trivial_assignment_operator());
		}
		
	};

	//__copt_backward_t trivial assignment
	template<class T>
	inline T *__copy_backward_t(const T* first,const T* last,T* res,__true_type){
		memmove(res-(last-first),first,sizeof(T)*(last-first));
		return res;
	}
	//__copt_backward_t non-trivial assignment
	template<class T>
	inline T *__copy_backward_t(const T* first,const T* last,T* res,__false_type){
		
		for(--last,--res;first!=last;--last,--res){
			*res = *last;
		}
		return res;
	}
/*******************************************************************************************************/
template<class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator first,Size n,const T& val){
	for(Size i = 0;i<n;i++,++first){
		*first = val;
	}	
	return first;
}
template<class ForwardIterator ,class T>
void fill(ForwardIterator first,ForwardIterator last,const T& val){
	while(first!=last){
		*first = val;
		++first;
	}
}
//char*
inline void fill(char* first, char* last, const char& c) {
	
	memset(first, c, last - first);
}

/***************************************************************************************************/



/***************************************************************************************************/
template <class InputIterator1,class InputIterator2>
inline bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2){
	while(first1!=last1){
		if(*first!=first2) return false;
		++first1;
		++first2;
	}
	return true;
}
template<class InputIterator1,class InputIterator2,class Com>
inline  bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,Com compare)(
	while(first1!=last1){
		if(!compare(*first1,*first2)){
			return false;
		}
		++first1;
		++first2;
	}
	return true;
}

/***************************************************************************************************/


}
#endif // !MYTINYSTL_ALGOBASE_H_
