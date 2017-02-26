#include"queue.h"
#include"vector.h"
#include<iostream>
using namespace jinstl;
int main(){
	priority_queue<int> pq;
	for(int i=1;i<10;i++) pq.push(i);
	while(!pq.empty()){
		std::cout<<pq.top();
		pq.pop();
	}
	std::cout<<std::endl;
	int a[]={1,2,3,4,5,6,7,8,9};
	vector<int> v(a,a+9);
	priority_queue<int> pq2(v.begin(),v.end());
	while(!pq2.empty()){
		std::cout<<pq2.top();
		pq2.pop();
	}
}
