#ifndef JIN_HEAP_H_
#define JIN_HEAP_H_
#include"type_traits.h"
#include"iterator.h"
namespace jinstl{

//up operator
template<class RandomAccessIterator,class Distance,class T>
void __push_heap(RandomAccessIterator first,Distance holeIndex,Distance topIndex,T value){
	Distance parentIndex = (holeIndex-1)/2;
	while(parentIndex>topIndex&&*(first+parentIndex)<value){
		*(first+holeIndex) = *(first+parentIndex);
		holeIndex = parentIndex;
		parentIndex = (holeIndex-1)/2;
	}
	*(first+holeIndex) = value;
	
	
}

//value is already in (last -1)
template<class RandomAccessIterator>
void push_heap(RandomAccessIterator first,RandomAccessIterator last){
	__push_heap(first,static_cast<typename iterator_traits<RandomAccessIterator>::difference_type>(last-first-1),static_cast<typename iterator_traits<RandomAccessIterator>::difference_type>(0),static_cast<typename iterator_traits<RandomAccessIterator>::value_type>(*(last-1)));
	
}
/*********************************************************/
template<class RandomAccessIterator,class Distance,class T,class Compare>
void __push_heap(RandomAccessIterator first,Distance holeIndex,Distance topIndex,T value,Compare com){
	Distance parentIndex = (holeIndex-1)/2;
	while(parentIndex>topIndex&&com(*(first+parentIndex),value)){
		*(first+holeIndex) = *(first+parentIndex);
		holeIndex = parentIndex;
		parentIndex = (holeIndex-1)/2;
	}
	*(first+holeIndex) = value;
}




template<class RandomAccessIterator,class Compare>
void push_heap(RandomAccessIterator first ,RandomAccessIterator last,Compare com){
	
	__push_heap(first,static_cast<typename iterator_traits<RandomAccessIterator>::difference_type>(last-first-1),static_cast<typename iterator_traits<RandomAccessIterator>::difference_type>(0),static_cast<typename iterator_traits<RandomAccessIterator>::value_type>(*(last-1)),com);

}
/**********************************************/
// down the unsuitable element
template<class RandomAccessIterator,class Distance,class T>
void __adjust_heap(RandomAccessIterator first,Distance holeIndex,Distance len,T value){
	Distance childIndex = holeIndex*2+2,topIndex = holeIndex;

	while(childIndex<len){
		if(*(first+childIndex)<*(first+(childIndex-1))) childIndex--;
		*(first+holeIndex)=*(first+childIndex);
		holeIndex = childIndex;
		childIndex = 2*holeIndex+2;
	}
	if(childIndex == len){//if  no right child
		*(first+holeIndex)=*(first+(childIndex-1));
		holeIndex = childIndex -1;
	}
	//*(first+holeIndex)=value;
	 __push_heap(first, holeIndex, topIndex, value);
}

template<class RandomAccessIterator,class T,class Distance>
void __pop_heap(RandomAccessIterator first,RandomAccessIterator last,RandomAccessIterator result,T value,Distance*){
	*result = *first;
	__adjust_heap(first,Distance(0),Distance(last-first),value);

}
template<class RandomAccessIterator >
void pop_heap(RandomAccessIterator first,RandomAccessIterator last){
	__pop_heap(first,last-1,last - 1,typename iterator_traits<RandomAccessIterator>::value_type(*(last-1)),static_cast<typename iterator_traits<RandomAccessIterator>::difference_type*>(0));//last-1 because *(last-1) is for result,so adjust [first,last-1) 
	
}

/********************************************************/

template<class RandomAccessIterator,class Distance,class T,class Compare>
void __adjust_heap(RandomAccessIterator first,Distance holeIndex,Distance len,T value,Compare com){
	Distance childIndex = holeIndex*2+2,topIndex = holeIndex;;

	while(childIndex<len){
		if(com(*(first+childIndex),*(first+childIndex-1))) childIndex--;
		*(first+holeIndex)=*(first+childIndex);
		holeIndex = childIndex;
		childIndex = 2*holeIndex+2;
	}
	if(childIndex == len){//if  no right child
		*(first+holeIndex)=*(first+(childIndex-1));
		holeIndex = childIndex -1;
	}
	//*(first+holeIndex)=value;
	__push_heap(first, holeIndex, topIndex, value,com);
}
template<class RandomAccessIterator,class T,class Distance,class Compare>
void __pop_heap(RandomAccessIterator first,RandomAccessIterator last,RandomAccessIterator result,T value,Distance*,Compare com){
	*result = *first;
	__adjust_heap(first,Distance(0),Distance(last-first),value,com);

}
template<class RandomAccessIterator,class Compare >
void pop_heap(RandomAccessIterator first,RandomAccessIterator last,Compare com){
	__pop_heap(first,last-1,last - 1,typename iterator_traits<RandomAccessIterator>::value_type(*(last-1)),static_cast<typename iterator_traits<RandomAccessIterator>::difference_type*>(0),com);//last-1 because *(last-1) is for result,so adjust [first,last-1) 
}	
/***********************************************************************/
template<class RandomAccessIterator,class T,class Distance>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last,T*,Distance*){
	Distance len = last - first;
	if(len <2) return;
	Distance holeIndex = (len-1)/2;
	while(true){
		__adjust_heap(first,holeIndex,len,typename iterator_traits<RandomAccessIterator>::value_type(*(first+holeIndex)));
		if(holeIndex==0) return;
		holeIndex--;
	}
	
}
template<class RandomAccessIterator>
void make_heap(RandomAccessIterator first,RandomAccessIterator last){
	
	__make_heap(first,last,static_cast<typename iterator_traits<RandomAccessIterator>::value_type*>(0),static_cast<typename iterator_traits<RandomAccessIterator>::difference_type*>(0));
}

/***********************************************/

template<class RandomAccessIterator,class T,class Distance,class Compare>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last,T*,Distance*,Compare com){
	Distance len = last - first;
	if(len <2) return;
	Distance holeIndex = (len-1)/2;
	while(true){
		__adjust_heap(first,holeIndex,len,typename iterator_traits<RandomAccessIterator>::value_type(*(first+holeIndex)),com);
		if(holeIndex==0) return;
		holeIndex--;
	}
	
}
template<class RandomAccessIterator,class Compare>
void make_heap(RandomAccessIterator first,RandomAccessIterator last,Compare com){
	
	__make_heap(first,last,static_cast<typename iterator_traits<RandomAccessIterator>::value_type*>(0),static_cast<typename iterator_traits<RandomAccessIterator>::difference_type*>(0),com);
}
/****************************************************/
template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first,RandomAccessIterator last){
	//make_heap(first,last);
	while(first!=last-1){
		pop_heap(first,last);
		last--;
	}
		
	
}

template<class RandomAccessIterator,class Compare>
void sort_heap(RandomAccessIterator first,RandomAccessIterator last,Compare com){
	while(first!=last-1){
		pop_heap(first,last,com);
		last--;
	}

}




}//namespace


#endif

