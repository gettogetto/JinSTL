#include<iostream>
#include"trietree.h"
using std::cout;
using std::endl;
using namespace jinstl;
void test1(){
    TrieTree tree;
    tree.addword("abcdefg");

    tree.addword("abc1111");
    tree.addword("ab1111");
    cout<<"size= "<<tree.size()<<endl;
    vector<string> res=tree.getword("abc");
    for(auto str:res){
        cout<<str<<endl;
    }
    cout<<tree.existed("123")<<endl;
    cout<<tree.existed("abcdefg")<<endl;

}
int main(){
    test1();
}
