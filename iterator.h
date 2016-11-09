#ifndef JINSTL_ITERATOR_H_
#define JINSTL_ITERATOR_H_


#include <stddef.h>	//for ptrdiff_t

namespace jinstl {

	// 5 kind of iterator
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	// input_iterator
	template <class T, class Distance>
	struct input_iterator {
		typedef input_iterator_tag			iterator_category;
		typedef T					value_type;
		typedef Distance				difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	// output_iterator
	template <class T, class Distance>
	struct output_iterator {
		typedef output_iterator_tag			iterator_category;
		typedef T					value_type;
		typedef Distance				difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	// forward_iterator
	template <class T, class Distance>
	struct forward_iterator {
		typedef forward_iterator_tag			iterator_category;
		typedef T					value_type;
		typedef Distance				difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	// bidirectional_iterator
	template <class T, class Distance>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag		iterator_category;
		typedef T					value_type;
		typedef Distance				difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	// random_access_iterator
	template <class T, class Distance>
	struct random_access_iterator {
		typedef random_access_iterator_tag		iterator_category;
		typedef T					value_type;
		typedef Distance				difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	// iterator template
	template <class Category, class T, class Distance = ptrdiff_t , class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category	iterator_category;
		typedef T		value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	// traits
	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type		value_type;
		typedef typename Iterator::difference_type	difference_type;
		typedef typename Iterator::pointer		pointer;
		typedef typename Iterator::reference		reference;
	};

	// special templated
	template <class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T				value_type;
		typedef ptrdiff_t			difference_type;
		typedef T*				pointer;
		typedef T&				reference;
	};

	template <class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag	iterator_category;
		typedef T				value_type;
		typedef ptrdiff_t			difference_type;
		typedef const T*			pointer;
		typedef const T&			reference;
	};

	// get category
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	// get distance type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//get value type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	// distance 
	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}

	// distance: input_iterator_tag 
	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		__distance(InputIterator first, InputIterator last, input_iterator_tag) {
        	typename iterator_traits<Inputerator>::difference_type n = 0;
		while(first!=last){
			++first;
			++n;
		}
		return n;
	}

	// distance: random_access_iterator_tag
	template <class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		__distance(RandomAccessIterator first, RandomAccessIterator last,
			random_access_iterator_tag) {
		return last - first;
	}

	// advance 
	template <class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n) {
		__advance(i, n, iterator_category(i));
	}

	// advance: input_iterator_tag 
	template <class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
		while (n--)	++i;
	}

	// advance:bidirectional_iterator_tag 
	template <class BidirectionalIterator, class Distance>
	inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--)	++i;
		else
			while (n++)	--i;
	}

	// advance: random_access_iterator_tag
	template <class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
		i += n;
	}
}

#include "reverse_iterator.h"	//  reverse_iterator

#endif // JINSTL_ITERATOR_H_

