#include"list.h"
#include<iostream>
#include<list>
#define out(l) 	std::cout<<#l<<": "; for(auto i:l) std::cout<<i<<" ";std::cout<<std::endl
int main(){
	int a[]={5,4,3,2,1,0},b[]={-1,-2,-3,-4,-5};
	jinstl::list<int> l1(a,a+6),l2=l1;
	out(l1);out(l2);
	l1.splice(l1.begin(),l1,++l1.begin(),l1.end());out(l1);out(l2);
	l1.insert(l1.begin(),-1);out(l1);
	l1.erase(l1.begin());out(l1);
	l1.remove(3);out(l1);
	l1.sort();out(l1);
	l1.unique();out(l1);
	l1.reverse();out(l1);
	auto l3=l1;l3.sort();l1.sort();
	l1.merge(l3);out(l1);
	l1.resize(20,0);out(l1);
	l1.resize(5);out(l1);
	
	jinstl::list<int> l4(a,a+6);
	jinstl::list<int> l5(a,a+6);
	std::cout<<(l4==l5)<<std::endl;

}
