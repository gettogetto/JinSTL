#include"list.h"
#include"testLib.h"
#include<iostream>
#include<list>
using namespace jinstl;
JINTEST(insert){
    int a[]={1,2,3};
    jinstl::list<int> l1(a,a+3);
    std::list<int> l2(a,a+3);
    EXPECT_CON_EQ(l1,l2);

    l1.insert(l1.begin(),0);
    l2.insert(l2.begin(),0);
    EXPECT_CON_EQ(l1,l2);

    l1.insert(l1.end(),2,-1);
    l2.insert(l2.end(),2,-1);
    EXPECT_CON_EQ(l1,l2);
}
JINTEST(push_pop){
    int a[]={1,2,3};
    jinstl::list<int> l1(a,a+3);
    std::list<int> l2(a,a+3);
    EXPECT_CON_EQ(l1,l2);

    l1.push_back(1);l2.push_back(1);EXPECT_CON_EQ(l1,l2);
    l1.push_front(1);l2.push_front(1);EXPECT_CON_EQ(l1,l2);
    l1.pop_back();l2.pop_back();EXPECT_CON_EQ(l1,l2);
    l1.pop_front();l2.pop_front();EXPECT_CON_EQ(l1,l2);
}
JINTEST(splice){
    int a[]={1,2,3};
    jinstl::list<int> l1(a,a+3);
    std::list<int> l2(a,a+3);
    int b[]={10,20,30};
    jinstl::list<int> l3(b,b+3);
    std::list<int> l4(b,b+3);
    EXPECT_CON_EQ(l1,l2);


    l1.splice(++l1.begin(),l3,l3.begin(),l3.end());
    l2.splice(++l2.begin(),l4,l4.begin(),l4.end());
    EXPECT_CON_EQ(l1,l2);

    l1.sort();
    l2.sort();
    EXPECT_CON_EQ(l1,l2);

}
JINTEST(sort){
    int a[]={1,2,3};
    jinstl::list<int> l1(a,a+3);
    std::list<int> l2(a,a+3);
    l1.sort();
    l2.sort();
    EXPECT_CON_EQ(l1,l2);
}

int main(){
    RUN_ALL_TESTS();
    return 0;
}
