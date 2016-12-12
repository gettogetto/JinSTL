#include"memory.h"
#include<iostream>
using std::cout;
using std::endl;
using namespace jinstl;
void test1(){

    shared_ptr<int> p(new int(123));
    cout<<"p"<<" "<<*p<<" "<<p.use_count()<<endl;

    shared_ptr<int>p2=p;
    cout<<"p"<<" "<<*p<<" "<<p.use_count()<<endl;
    cout<<"p2"<<" "<<*p2<<" "<<p2.use_count()<<endl;


    p=make_shared<int>(321);
    cout<<"p"<<" "<<*p<<" "<<p.use_count()<<endl;
    cout<<"p2"<<" "<<*p2<<" "<<p2.use_count()<<endl;
}

int main(){
    test1();
}
