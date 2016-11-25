#ifndef JIN_QUEUE_H
#define JIN_QUEUE_H
#include"list.h"
namespace jinstl{
template<class T,class Container = list<T> >
class queue{
public:
	typedef typename Container::value_type value_type;
	typedef typename Container::reference reference;
	typedef typename Container::const_reference const_reference;
	typedef size_t size_type;
	queue():_container(){}
	~queue(){}
	void push(const T& val){
		_container.insert(_container.begin(),val);
	}
	T& front(){
		return *_container.begin();
	}
	T& back(){
		return *(--_container.end());

	}
	void pop(){
		_container.erase(_container.begin());
	}

	bool empty(){return _container.empty();}
	size_type size(){return _container.size();}
	
private:
	Container _container;
	friend bool operator ==(const queue&q1,const queue&q2){return q1._container==q2._container;}
	friend bool operator <(const queue&q1,const queue&q2){return q1._container<q2._container;}
	friend bool operator !=(const queue&q1,const queue&q2){return !(q1==q2);}
	friend bool operator >(const queue&q1,const queue&q2){return !(q1<q2);}
};


}//namespace jinstl

#endif

