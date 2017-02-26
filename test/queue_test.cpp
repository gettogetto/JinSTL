#include"queue.h"
#include"testLib.h"
#include<queue>
#include<iostream>
using namespace jinstl;
JINTEST(push){
    jinstl::queue<int> s1;
    std::queue<int> s2;
    s1.push(1);
    s2.push(1);
    EXPECT_EQ(s1.front(),s2.front());
    s1.push(2);
    s2.push(2);
    EXPECT_EQ(s1.front(),s2.front());
    s1.pop();
    s2.pop();

    EXPECT_EQ(s1.front(),s2.front());
}

int main(){
    RUN_ALL_TESTS();
    return 0;
}
