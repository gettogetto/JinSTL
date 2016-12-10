#ifndef JIN_UNION_FIND_H
#define JIN_UNION_FIND_H
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
namespace jinstl{
template<size_t N>
class unionfind{
public:
    unionfind():_components(N){
        for(size_t i=0;i<N;i++) _parents[i]=-1;//-n stands for i is root and has n nodes

    }
    void connect(size_t node1,size_t node2){
        size_t root1=findRoot(node1);
        size_t root2=findRoot(node2);
        if(root1==root2) return;
        int totalnodes=_parents[root1]+_parents[root2];
        if(abs(_parents[root1])>abs(_parents[root2])){
            _parents[root2]=root1;
            _parents[root1]=totalnodes;

        }else{
            _parents[root1]=root2;
            _parents[root2]=totalnodes;
        }
        _components--;
    }

    size_t findRoot(size_t node){
        size_t root = node;
        for(;_parents[root]>=0;root=_parents[root]){}
        while(node!=root){//path compress
            _parents[node]=root;
            node=_parents[node];
        }
        return root;
    }

    bool isConnected(size_t node1,size_t node2){
        return findRoot(node1)==findRoot(node2);
    }
    size_t getComponents(){
        return _components;
    }

private:
    size_t _components;
    int _parents[N];

};
}


#endif
