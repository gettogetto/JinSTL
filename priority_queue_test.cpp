#include"queue.h"
#include"vector.h"
#include<iostream>
using namespace jinstl;
int main(){
	priority_queue<int> pq;
	for(int i=1;i<10;i++) pq.push(i);
	while(!pq.empty()){
		std::cout<<pq.top()<<std::endl;
		pq.pop();
	}
}
