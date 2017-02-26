#include<iostream>
#include"deque.h"
#include<deque>
#include"testLib.h"
using namespace jinstl;
JINTEST(insert){
    int a[]={1,2,3};
    jinstl::deque<int> v1(a,a+3);
    std::deque<int> v2(a,a+3);
    v1.insert(v1.begin(),0);
    v2.insert(v2.begin(),0);
    EXPECT_CON_EQ(v1,v2);

    v1.insert(v1.end(),4);
    v2.insert(v2.end(),4);
    EXPECT_CON_EQ(v1,v2);

    v1.insert(v1.end(),6,-1);
    v2.insert(v2.end(),6,-1);
    EXPECT_CON_EQ(v1,v2);
}

JINTEST(erase){
    int a[]={1,2,3};
    jinstl::deque<int> v1(a,a+3);
    std::deque<int> v2(a,a+3);
    v1.erase(v1.begin());
    v2.erase(v2.begin());
    EXPECT_CON_EQ(v1,v2);

    v1.erase(v1.end()-1);
    v2.erase(v2.end()-1);
    EXPECT_CON_EQ(v1,v2);
}

JINTEST(push_and_pop){
    int a[]={1,2,3};
    jinstl::deque<int> v1(a,a+3);
    std::deque<int> v2(a,a+3);
    v1.push_back(4);
    v2.push_back(4);
    EXPECT_CON_EQ(v1,v2);

    v1.pop_back();
    v2.pop_back();
    EXPECT_CON_EQ(v1,v2);
}


JINTEST(resize){
    int a[]={1,2,3};
    jinstl::deque<int> v1(a,a+3);
    std::deque<int> v2(a,a+3);

    v1.resize(2);
    v2.resize(2);
    EXPECT_CON_EQ(v1,v2);

    v1.resize(5);
    v2.resize(5);
    EXPECT_CON_EQ(v1,v2);
}

int main(){
    RUN_ALL_TESTS();
    return 0;
}
