#include"string.h"
#include<iostream>
using namespace jinstl;
using std::cout;
using std::endl;
//using std::string;
#define out(a) cout<<a<<endl
int main(){
    string a="1234567";
    string b="abcdefg";
    std::cout<<std::boolalpha<<(a>b)<<endl;
    a.append("321");out(a);
    a.insert(a.begin(),"aaa","aaa"+3);out(a);

    a.replace(a.begin(),a.begin()+3,"xyz");out(a);
    a.reserve(100);
    cout<<a.capacity()<<endl;out(a);

    cout<<a.compare("xyz1234")<<endl;

    a.erase(a.begin(),a.begin()+3);out(a);

    a.push_back('e');out(a);

    a.shrink_to_fit();
    cout<<a.capacity()<<endl;out(a);

    a.pop_back();out(a);
    string tmp="123";
    a.swap(tmp);out(a);out(tmp);

}
