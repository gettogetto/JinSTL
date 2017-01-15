#ifndef TESTLIB_H
#define TESTLIB_H
#include<iostream>
#include<string>
#include<vector>
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;
inline ostream& RED(ostream& out){
    out<<"\033[91m";
    return out;
}
inline ostream& GREEN(ostream& out){
    out<<"\033[92m";
    return out;
}

class TestCase{
public:
    TestCase(const string& str):_name(str){}
    virtual void run()=0;

    string _name;
    int _nResult=1;
    double _nFailed=0;
    double _nPassed=0;
};

class unitTest{
public:
    static unitTest& getInstance(){
        static unitTest ut;
        return ut;
    }

    TestCase * addTestcase(TestCase *testcase){
        _testcases.push_back(testcase);
        return testcase;
    }

    void run(){
        for(auto it:_testcases){
            TestCase* t=it;
            _curTestCase=t;
            //t->_nResult=1;
            //t->_nPassed=t->_nFailed=0;
            cout<<GREEN<<"============================="<<endl;
            cout<<GREEN<<"Run TestCase "<<t->_name<<endl;
            t->run();
            if(t->_nFailed==0){
                cout<<GREEN;
            }else{
                cout<<RED;
            }
            cout<<"Test Result: "<<t->_nPassed<<'/'<<t->_nFailed+t->_nPassed<<"("<<t->_nPassed/(t->_nPassed+t->_nFailed)*100<<"%)"<<" passed"<<endl;
            if(t->_nResult==1){
                _nPassed++;
            }else{
                _nFailed++;
            }
        }
        cout<<GREEN<<"============================="<<endl;
        if(_nFailed==0){
            cout<<GREEN;
        }else{
            cout<<RED;
        }
        cout<<"Total Test Result: "<<_nPassed<<'/'<<_nFailed+_nPassed<<"("<<_nPassed/(_nPassed+_nFailed)*100<<"%)"<<" passed"<<endl;
    }

public:
    int _nResult;
    double _nPassed;
    double _nFailed;
    TestCase* _curTestCase;
private:
    unitTest(){}
    vector<TestCase*> _testcases;

};
#define TESTCASE_NAME(testcase_name)\
    testcase_name##_TEST

#define JINTEST(testcase_name)\
    class TESTCASE_NAME(testcase_name):public TestCase{\
    public:\
        TESTCASE_NAME(testcase_name)(const string& name):\
            TestCase(name){}\
        virtual void run();\
    private:\
        static TestCase* const _testcase;\
    };\
    TestCase* const TESTCASE_NAME(testcase_name)\
    ::_testcase=unitTest::getInstance().addTestcase(new TESTCASE_NAME(testcase_name)(#testcase_name));\
    void TESTCASE_NAME(testcase_name)::run()
//static TestCase* const _testcase; in order to run the "addTestcase"
#define EXPECT_TRUE(Condition) do {                                    \
    if (Condition) {                                                    \
        unitTest::getInstance()._curTestCase->_nPassed++;            \
        std::cout << GREEN << " EXPECT_TRUE succeeded!" << std::endl;   \
    }                                                                   \
    else {                                                              \
        unitTest::getInstance()._curTestCase->_nResult = 0;      \
        unitTest::getInstance()._curTestCase->_nFailed++;            \
        std::cout << RED << " EXPECT_TRUE failed!" << std::endl;        \
}} while(0)

#define EXPECT_FALSE(Condition) do {                                   \
    if (!Condition) {                                                   \
        unitTest::getInstance()._curTestCase->_nPassed++;            \
        std::cout << GREEN << " EXPECT_FALSE succeeded!" << std::endl;  \
    }                                                                   \
    else {                                                              \
        unitTest::getInstance()._curTestCase->_nResult = 0;      \
        unitTest::getInstance()._curTestCase->_nFailed++;            \
        std::cout << RED << " EXPECT_FALSE failed!" << std::endl;      \
}} while(0)

#define EXPECT_EQ(v1, v2) do { \
    if (v1 == v2) {                                                 \
        unitTest::getInstance()._curTestCase->_nPassed++;         \
        std::cout << GREEN << " EXPECT_EQ succeeded!" << std::endl;  \
    }                                                                \
    else {                                                           \
        unitTest::getInstance()._curTestCase->_nResult = 0;   \
        unitTest::getInstance()._curTestCase->_nFailed++;         \
        std::cout << RED << " EXPECT_EQ failed!" << std::endl;       \
        std::cout << RED << " Expect:" << v1 << std::endl;           \
        std::cout << RED << " Actual:" << v2 << std::endl;           \
}} while(0)

#define EXPECT_NE(v1, v2) do {                                      \
    if (v1 != v2) {                                                  \
        unitTest::getInstance()._curTestCase->_nPassed++;         \
        std::cout << GREEN << " EXPECT_NE succeeded!" << std::endl;  \
    }                                                                \
    else {                                                           \
        unitTest::getInstance()._curTestCase->_nResult = 0;   \
        unitTest::getInstance()._curTestCase->_nFailed++;         \
        std::cout << RED << " EXPECT_NE failed!" << std::endl;       \
        std::cout << RED << " Expect:" << v1 << std::endl;           \
        std::cout << RED << " Actual:" << v2 << std::endl;           \
}} while(0)

#define EXPECT_LT(v1, v2) do {                                      \
    if (v1 < v2) {                                                   \
        unitTest::getInstance()._curTestCase->_nPassed++;         \
        std::cout << GREEN << " EXPECT_LT succeeded!" << std::endl;  \
    }                                                                \
    else {                                                           \
        unitTest::getInstance()._curTestCase->_nResult = 0;   \
        unitTest::getInstance()._curTestCase->_nFailed++;         \
        std::cout << RED << " EXPECT_LT failed!" << std::endl;       \
        std::cout << RED << " Expect:" << v1 << std::endl;           \
        std::cout << RED << " Actual:" << v2 << std::endl;           \
}} while(0)

#define EXPECT_LE(v1, v2) do {                                      \
    if (v1 <= v2) {                                                  \
        unitTest::getInstance()._curTestCase->_nPassed++;         \
        std::cout << GREEN << " EXPECT_LE succeeded!" << std::endl;  \
    }                                                                \
    else {                                                           \
        unitTest::getInstance()._curTestCase->_nResult = 0;   \
        unitTest::getInstance()._curTestCase->_nFailed++;         \
        std::cout << RED << " EXPECT_LE failed!" << std::endl;       \
        std::cout << RED << " Expect:" << v1 << std::endl;           \
        std::cout << RED << " Actual:" << v2 << std::endl;           \
}} while(0)

#define EXPECT_GT(v1, v2) do {                                      \
    if (v1 > v2) {                                                   \
        unitTest::getInstance()._curTestCase->_nPassed++;         \
        std::cout << GREEN << " EXPECT_GT succeeded!" << std::endl;  \
    }                                                                \
    else {                                                           \
        unitTest::getInstance()._curTestCase->_nResult = 0;   \
        unitTest::getInstance()._curTestCase->_nFailed++;         \
        std::cout << RED << " EXPECT_GT failed!" << std::endl;       \
        std::cout << RED << " Expect:" << v1 << std::endl;           \
        std::cout << RED << " Actual:" << v2 << std::endl;           \
}} while(0)

#define EXPECT_GE(v1, v2) do {                                      \
    if (v1 >= v2) {                                                  \
        unitTest::getInstance()._curTestCase->_nPassed++;         \
        std::cout << GREEN << " EXPECT_GE succeeded!" << std::endl;  \
    }                                                                \
    else {                                                           \
        unitTest::getInstance()._curTestCase->_nResult = 0;   \
        unitTest::getInstance()._curTestCase->_nFailed++;         \
        std::cout << RED << " EXPECT_GE failed!" << std::endl;       \
        std::cout << RED << " Expect:" << v1 << std::endl;           \
        std::cout << RED << " Actual:" << v2 << std::endl;           \
}} while(0)



#define EXPECT_CON_EQ(c1, c2) do {                                      \
    auto first1 = std::begin(c1), last1 = std::end(c1);                  \
    auto first2 = std::begin(c2), last2 = std::end(c2);                  \
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {     \
        if (*first1 != *first2)  break;                                  \
    }                                                                    \
    if (first1 == last1 && first2 == last2) {                            \
        unitTest::getInstance()._curTestCase->_nPassed++;             \
        std::cout << GREEN << " EXPECT_CON_EQ succeeded!" << std::endl;  \
    }                                                                    \
    else {                                                               \
        unitTest::getInstance()._curTestCase->_nResult = 0;       \
        unitTest::getInstance()._curTestCase->_nFailed++;             \
        std::cout << RED << " EXPECT_CON_EQ failed!" << std::endl;       \
        std::cout << RED << " Expect:";SHOW_CONTAINER(c2);         \
        std::cout << RED << " Actual:";SHOW_CONTAINER(c1);          \
}} while(0)

#define EXPECT_CON_NE(c1, c2) do {                                      \
    auto first1 = std::begin(c1), last1 = std::end(c1);                  \
    auto first2 = std::begin(c2), last2 = std::end(c2);                  \
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {     \
        if (*first1 != *first2)  break;                                  \
    }                                                                    \
    if (first1 != last1 || first2 != last2) {                            \
        unitTest::getInstance()._curTestCase->_nPassed++;             \
        std::cout << GREEN << " EXPECT_CON_NE succeeded!" << std::endl;  \
    }                                                                    \
    else {                                                               \
        unitTest::getInstance()._curTestCase->_nResult = 0;       \
        unitTest::getInstance()._curTestCase->_nFailed++;             \
        std::cout << RED << " EXPECT_CON_NE failed!" << std::endl;       \
        std::cout << RED << " Expect:" ;SHOW_CONTAINER(c2);\
        std::cout << RED << " Actual:" ;SHOW_CONTAINER(c1); \
}} while(0)

#define SHOW_CONTAINER(containername)\
    do{\
        std::cout<<#containername<<" :";\
        for(auto it:containername){\
            std::cout<<" "<<it;\
        }\
        std::cout<<std::endl;\
    }while(0)

#define SHOW_CONTAINER_AFTER_FUNC(func,container)\
    do{\
        std::cout<<"After function: "<<#func<<std::endl;\
        func;\
        SHOW_CONTAINER(container);\
    }while(0)


#define RUN_ALL_TESTS() \
    do{\
        unitTest::getInstance().run();\
    }while(0)

#endif
