#include"deque.h"
#include<iostream>
using namespace jinstl;
#define out(v) for(int i=0;i<(int)v.size();i++) std::cout<<*(v.begin()+i)<<" ";std::cout<<"   capacity="<<v.size()<<std::endl
int main(){
	deque<int> q;
	q.insert(q.begin(),1);
	out(q);

	int a[]={1,2,3,4};
	deque<int> q2(a,a+4);
	out(q2);

	q.insert(q.begin(),5,2);
	out(q);

	q.push_front(1);
	out(q);

}
