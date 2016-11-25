#include<iostream>
#include"vector.h"

#define out(v) for(int i=0;i<(int)v.size();i++) std::cout<<*(v.begin()+i);std::cout<<"capacity="<<v.capacity()<<std::endl
using namespace jinstl;
int main(){
	vector<int> v(1),v2(10,5);
	out(v);
	v.pop_back();
	out(v);
	v.insert(v.begin(),7);
	out(v);
	v.insert(v.begin(),v2.begin(),v2.end());
	out(v);
	for(int i=0;i<10;i++) v.push_back(9-i);
	out(v);
	return 0;	

}
