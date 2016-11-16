#include<iostream>
#include"vector.h"
#define out() for(int i=0;i<(int)v.size();i++) std::cout<<*(v.begin()+i);std::cout<<"capacity="<<v.capacity()<<std::endl
using namespace jinstl;
int main(){
	vector<int> v(1),v2(10,5);
	out();
	v.pop_back();
	out();
	v.insert(v.begin(),7);
	out();
	v.insert(v.begin(),v2.begin(),v2.end());
	out();
	for(int i=0;i<10;i++) v.push_back(9-i);
	out();
	return 0;	

}
