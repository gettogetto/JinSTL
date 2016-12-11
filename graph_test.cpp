#include"graph.h"
#include<iostream>

using namespace jinstl;
using std::cout;
using std::endl;



void test2(){
    graph<int,char> g;

    auto node0=g.makeNode(0,'a');
    vector<typename graph<int,char>::Node> vec0;
    vec0.push_back(g.makeNode(1,'b'));
    g.addNode(node0,vec0);

    auto node1=g.makeNode(1,'b');
    vector<typename graph<int,char>::Node> vec1;
    vec1.push_back(g.makeNode(0,'a'));
    g.addNode(node1,vec1);

    auto func =[](const typename graph<int,char>::Node& node){cout<<"["<<node.first<<","<<node.second<<"]"<<endl;};
    //(*g.begin(0)).first;
    //std::cout<<(*g.begin(1)).first<<(*g.begin(1)).second;


    //for(auto it=g.begin(0);it!=g.end(0);++it) func(*it);
    g.DFS(1,func);
    g.BFS(1,func);
}

int main(){
    test2();

}
