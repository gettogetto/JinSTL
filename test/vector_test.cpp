#include<iostream>
#include"vector.h"
#include<vector>
#include<functional>
#include"testLib.h"
using namespace jinstl;
JINTEST(insert){
    int a[]={1,2,3};
    jinstl::vector<int> v1(a,a+3);
    std::vector<int> v2(a,a+3);
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
    jinstl::vector<int> v1(a,a+3);
    std::vector<int> v2(a,a+3);
    v1.erase(v1.begin());
    v2.erase(v2.begin());
    EXPECT_CON_EQ(v1,v2);

    v1.erase(v1.end()-1);
    v2.erase(v2.end()-1);
    EXPECT_CON_EQ(v1,v2);
}


JINTEST(push_and_pop){
    int a[]={1,2,3};
    jinstl::vector<int> v1(a,a+3);
    std::vector<int> v2(a,a+3);
    v1.push_back(4);
    v2.push_back(4);
    EXPECT_CON_EQ(v1,v2);

    v1.pop_back();
    v2.pop_back();
    EXPECT_CON_EQ(v1,v2);
}

JINTEST(resize){
    int a[]={1,2,3};
    jinstl::vector<int> v1(a,a+3);
    std::vector<int> v2(a,a+3);

    v1.resize(2);
    v2.resize(2);

    EXPECT_CON_EQ(v1,v2);

    v1.resize(5);
    v2.resize(5);
    EXPECT_CON_EQ(v1,v2);
}
JINTEST(push_back_timecost){
    const long count=100000000;
    jinstl::vector<int> v1;
    std::vector<int> v2;
    TIMECOST(v1,push_back,rand(),count);
    TIMECOST(v2,push_back,rand(),count);
}
JINTEST(insert_timecost){
    const long count=100000;
    jinstl::vector<int> v1;
    std::vector<int> v2;
    TIMECOST2(v1,insert,begin,rand(),count);
    TIMECOST2(v2,insert,begin,rand(),count);
}
int main(){
    RUN_ALL_TESTS();
	return 0;	
}
