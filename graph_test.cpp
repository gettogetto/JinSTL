#include"graph.h"
#include<iostream>

using namespace jinstl;
using std::cout;
using std::endl;



void test2(){
    graph<int,char> g;

    auto node0=g.makeNode(0,'a');
    auto node1=g.makeNode(1,'b');

    vector<typename graph<int,char>::Node> vec0,vec1;
    g.addNode(node0,vec0);
    g.addNode(node1,vec1);
    g.makeEdge(0,1);
    g.makeEdge(1,0);
    auto func =[](const typename graph<int,char>::Node& node){cout<<"["<<node.first<<","<<node.second<<"]"<<endl;};
    g.DFS(1,func);
    g.BFS(1,func);
}

int main(){
    test2();

}
