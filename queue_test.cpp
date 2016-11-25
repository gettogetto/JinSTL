#include"queue.h"
#include<iostream>
using namespace jinstl;
int main(){
	queue<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
	s.push(4);
	s.push(5);
	std::cout<<s.front()<<std::endl;
	std::cout<<s.back()<<std::endl;
	std::cout<<(s==s)<<std::endl;
	s.pop();
	std::cout<<s.front()<<std::endl;
	s.pop();
	std::cout<<s.front()<<std::endl;
	s.pop();
	std::cout<<s.front()<<std::endl;
	s.pop();
	std::cout<<s.front()<<std::endl;
	s.pop();
	std::cout<<s.empty()<<std::endl;
}
