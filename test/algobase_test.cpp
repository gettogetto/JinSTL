#include"iterator.h"
#include"vector.h"
#include<vector>
#include"testLib.h"

using namespace jinstl;
JINTEST(advance){
	int a[]={1,2,3,4,5};
	jinstl::vector<int> v1(a,a+5);
	std::vector<int> v2(a,a+5);
	auto it1=v1.begin();
	auto it2=v2.begin();
	jinstl::advance(it1,3);
	std::advance(it2,3);
	EXPECT_EQ(*it1,*it2);
}
JINTEST(distance){
	int a[]={1,2,3,4,5};
	jinstl::vector<int> v1(a,a+5);
	std::vector<int> v2(a,a+5);
	auto d1=jinstl::distance(v1.begin(),v1.end());
	auto d2=std::distance(v2.begin(),v2.end());
	EXPECT_EQ(d1,d2);
}
int main(){
	RUN_ALL_TESTS();
	return 0;
}
