#ifndef JIN_STL_ALGOBASE_H_
#define JIN_STL_ALGOBASE_H_



#include <string.h>

#include "iterator.h"
#include "pair.h"
#include "type_traits.h"

namespace jinstl {

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
	// copy
	//  [first, last) to [result, result + (last - first))内
	/*********************************************************************************/

	template <class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy_dispatch < InputIterator, OutputIterator>()(first, last, result);
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
	// copy_backward
	// copy [first, last)  to [result - (last - first), result)
	/*********************************************************************************/


	
	// __copy_backward bidirectional_iterator_tag 
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result,
		bidirectional_iterator_tag, Distance*) {
		while (first != last) {
			*--result = *--last;
		}
		return result;
	}
	
	// __copy_backward  random_access_iterator_tag 
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result,
		random_access_iterator_tag, Distance*) {
		for (Distance n = last - first; n > 0; --n) {
			*--result = *--last;
		}
		return result;
	}


//  __copy_backward_dispatch 
	template <class BidirectionalIterator1, class BidirectionalIterator2, class Type>
	struct __copy_backward_dispatch {
		typedef typename iterator_traits<BidirectionalIterator1>::iterator_category Category;
		typedef typename iterator_traits<BidirectionalIterator1>::difference_type Distance;

		static BidirectionalIterator2 copy(BidirectionalIterator1 first,
			BidirectionalIterator1 last, BidirectionalIterator2 result) {
			return __copy_backward(first, last, result, Category(), static_cast<Distance*>(0));
		}
	};

	// __copy_backward_dispatch  T*
	template <class T>
	struct __copy_backward_dispatch<T*, T*, __true_type> {
		static T* copy(const T* first, const T* last, T* result) {
			const T n = last - first;
			memmove(result - n, first, sizeof(T) * n);
			return result - n;
		}
	};

	// __copy_backward_dispatch  const T*， T*
	template <class T>
	struct __copy_backward_dispatch<const T*, T*, __true_type> {
		static T* copy(const T* first, const T* last, T* result) {
			return __copy_backward_dispatch<T*, T*, __true_type>
				::copy(first, last, result);
		}
	};

	template <class BidirectionalIterator1, class BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result) {
		typedef typename __type_traits<typename iterator_traits<BidirectionalIterator2>::value_type>
			::has_trivial_assignment_operator	Trivial;
		return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2, Trivial()>
			::copy(first, last, result);
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
template <class InputIterator1,class InputIterator2>
inline bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2){
	while(first1!=last1){
		if(*first1!=*first2) return false;
		++first1;
		++first2;
	}
	return true;
}
template<class InputIterator1,class InputIterator2,class Com>
inline  bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,Com compare){
	while(first1!=last1){
		if(!compare(*first1,*first2)){
			return false;
		}
		++first1;
		++first2;
	}
	return true;
}
template <class InputIterator1,class InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2){
	while(first1!=last1&&first2!=last2){
		if(*first1<*first2) return true;
		if(*first1>*first2) return false;
		first1++;
		first2++;
	}
	return first1==last1&&first2!=last2;//shorter

}
template <class InputIterator1,class InputIterator2,class Com>
inline bool lexicographical_compare(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,Com compare){
	while(first1!=last1&&first2!=last2){
		if(compare(*first1,*first2)) return true;
		if(compare(*first1,*first2)) return false;
		first1++;
		first2++;
	}
	return first1==last1&&first2!=last2;//shorter
	

}
template <class InputIterator1,class InputIterator2>
inline int lexicographical_compare_3way(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2){
	while(first1!=last1&&first2!=last2){
		if(*first1<*first2) return -1;
		if(*first1>*first2) return 1;
		first1++;
		first2++;
	}
	if(first1==last1){
		return !(first2==last2);
	}else{
		return 1;
	}

}
/***************************************************************************************************/
	template<class T>
	inline void swap(T& t1,T& t2){
		T tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	template<class T>
	inline const T& max(const T& t1,const T& t2){
		return t1<t2?t2:t1;	
	}
	template<class T>
	inline const T& min(const T& t1,const T& t2){
		return t1<t2?t1:t2;
	}
	template<class T,class compare>
	inline const T& max(T& t1,T& t2){
		return compare(t1,t2)?t2:t1;	
	}
	template<class T,class compare>
	inline const T& min(T& t1,T& t2){
		return compare(t1,t2)?t1:t2;
	}
}
#endif // 
