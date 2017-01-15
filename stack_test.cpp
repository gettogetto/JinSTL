#include"stack.h"
#include"testLib.h"
#include<stack>
#include<iostream>
using namespace jinstl;
JINTEST(push){
    jinstl::stack<int> s1;
    std::stack<int> s2;
    s1.push(1);
    s2.push(1);
    EXPECT_EQ(s1.top(),s2.top());
    s1.push(2);
    s2.push(2);
    EXPECT_EQ(s1.top(),s2.top());
    s1.pop();
    s2.pop();
    EXPECT_EQ(s1.top(),s2.top());
}

int main(){
    RUN_ALL_TESTS();
    return 0;
}
