#include"unionfind.h"
#include<iostream>
using namespace jinstl;
int main(){
    unionfind<10> uf;
    uf.connect(0,1);
    uf.connect(1,2);
    uf.connect(2,3);
    std::cout<<uf.getComponents()<<std::endl;
}
