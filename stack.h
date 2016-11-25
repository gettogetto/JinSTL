#ifndef JIN_STACK_H_
#define JIN_STACK_H_
#include"list.h"
namespace jinstl{
	template<class T,class Container=list<T>>
	class stack{
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::size_type size_type;
		typedef typename Container::const_reference const_reference;
		stack():_container(){}
		~stack(){}
		void push(const value_type& val){
			_container.insert(_container.end(),val);
		}
		void pop(){
			_container.erase(--_container.end());
		}
		T& top(){
			return *(--_container.end());
		}
		bool empty(){
			return _container.empty();
		}
		size_type size(){
			return _container.size();
		}

	private:
		Container _container;
		
		/*friend bool operator == (const stack &s1,const stack &s2){

			return s1._container==s2._container;
		}
		
		friend bool operator != (const stack &s1,const stack &s2){

			return !(s1==s2);
		}
		
		friend bool operator <(const stack&s1,const stack &s2){

			return s1._container<s2._container;
		}*/

	};//class stack
		template<class T,class Container>
 		bool operator == (const stack<T,Container> &s1,const stack<T,Container> &s2){

			return s1._container==s2._container;
		}
		template<class T,class Container>
		 bool operator != (const stack<T,Container> &s1,const stack<T,Container> &s2){

			return !(s1==s2);
		}
		template<class T,class Container>
		 bool operator <(const stack<T,Container> &s1,const stack<T,Container> &s2){

			return s1._container<s2._container;
		}

}//namespace jinstl
#endif
