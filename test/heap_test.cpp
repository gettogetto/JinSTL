
#include"heap.h"
#include<iostream>
#include"vector.h"
using namespace jinstl;
#define out(v) do{for(auto i=v.begin();i!=v.end();i++) std::cout<<*i<<" ";std::cout<<std::endl;}while(0)
int main(){
	int a[]={1,4,2,1,2,3,2,1,5};
	vector<int> v(a,a+9);
	make_heap(v.begin(),v.end());	
	out(v);
	sort_heap(v.begin(),v.end());
	out(v);
	
}
