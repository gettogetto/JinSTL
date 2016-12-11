#ifndef JIN_GRAPH_H
#define JIN_GRAPH_H
#include<utility>
#include<vector>//to do with "vector.h"
using std::vector;
using std::pair;

#include"iterator.h"
#include<functional>
#include<unordered_set>



namespace jinstl{

template<class Index,class Value>
class innerIterator;

template<class Index,class Value>
class outerIterator;

template<class Index,class Value>
class graph{
    friend class innerIterator<Index,Value>;
    friend class outerIterator<Index,Value>;
public:
    typedef pair<Index,Value> Node;
    typedef vector<Node> NodeVec;
    typedef vector< pair<Node,vector<Node> > > AdjList;
    typedef outerIterator<Index,Value> iterator;
    typedef innerIterator<Index,Value> inner_iterator;


    graph():_size(0){

    }
    ~graph(){

    }
    Node makeNode(const Index& i,const Value& j){
        return Node(i,j);
    }

    const Node& getNode(const Index& i){
        for(auto &pair:_nodes){//must &
            if(pair.first.first==i){
                return pair.first;
            }
        }
        //assert(0);
        static Node emptynode;
        return emptynode;
    }

    bool hasNode(const Index& i){
        for(auto &pair:_nodes){//must &
            if(pair.first.first==i){
                return true;
            }
        }
        return false;
    }

    void addNode(const Node& node,const NodeVec& vec){
        if(!hasNode(node.first)){
            _nodes.push_back(make_pair(node,vector<Node>()));
            ++_size;
        }
        addNode(node.first,vec);
    }

    void addNode(const Index& i,const NodeVec& vec){
        if(vec.empty()) return;
        vector<typename graph::Node>* plist;
        for(auto &pa:_nodes){//must &
            if(pa.first.first==i){
                plist=&(pa.second);
                break;
            }
        }
        for(const auto node:vec){
            plist->push_back(node);
            if(!hasNode(node.first)){
                addNode(node,vector<Node>());
            }
        }
    }

    void makeEdge(const Node& node1,const Node& node2){
        for(auto it=_nodes.begin();it!=_nodes.end();it++){
            if((it->first)==node1){
                it->second.push_back(node2);
            }
        }
    }

    void makeEdge(const Index& i1,const Index& i2){
        auto node1=getNode(i1);
        auto node2=getNode(i2);
        for(auto it=_nodes.begin();it!=_nodes.end();it++){
            if((it->first).first==i1){
                it->second.push_back(node2);
            }
        }
    }

    void deleteNode(const Node& node){
        deleteNode(node.first);
    }

    void deleteNode(const Index& i){
        for(auto out_it=_nodes.begin();out_it!=_nodes.end();){
            if(out_it->first.first==i){
                out_it=_nodes.erase(out_it);
            }else{
                for(auto in_it=out_it->second.begin();in_it!=out_it->second.end();){
                    if(in_it->first==i){
                        in_it=out_it->second.erase(in_it);
                    }else{
                        in_it++;
                    }
                }
                out_it++;
            }
        }
    }

    NodeVec adjacentNodes(const Node& node){
        return adjacentNodes(node.first);
    }

    NodeVec adjacentNodes(const Index& i){
        NodeVec res;
        for(auto it=begin(i);it!=end(i);it++){
            res.push_back(*it);
        }
        return res;
    }

    void BFS(const Index& i,std::function<void(const Node&)> func){
        std::unordered_set<Index> visited;

        _BFS_aux(getNode(i),func,visited);
    }

    void DFS(const Index& i,std::function<void(const Node&)> func){
        std::unordered_set<Index> visited;

        _DFS_aux(getNode(i),func,visited);
    }

    bool empty(){
        return _nodes.empty();
    }

    size_t size(){
        return _size;
    }

    inner_iterator begin(const Index& i){
        for(auto &pa:_nodes){
            if(pa.first.first==i){
                return inner_iterator(this,(pa.second).begin());
            }
        }
        return end(i);
    }

    inner_iterator end(const Index& i){
        for(auto &pa:_nodes){
            if(pa.first.first==i){
                return inner_iterator(this,(pa.second).end());
            }
        }
        return inner_iterator();
    }


    iterator begin(){
        return iterator(this,_nodes.begin());
    }

    iterator end(){
        return iterator(this,_nodes.end());
    }

protected:

    void _BFS_aux(const typename graph::Node& node,std::function<void(const typename graph::Node&)> func,std::unordered_set<Index>&visited){
        auto nodeVec=adjacentNodes(node);
        func(node);
        visited.insert(node.first);
        do{
            NodeVec tmp;
            for(auto it=nodeVec.begin();it!=nodeVec.end();it++){
                if(visited.count(it->first)==0){//not visited
                    func(*it);
                    visited.insert(it->first);
                    auto s=adjacentNodes(*it);
                    tmp.insert(tmp.end(),s.begin(),s.end());
                }
            }
            nodeVec=tmp;
        }while(!nodeVec.empty());
    }

    void _DFS_aux(const Node& node,std::function<void(const Node&)> func,std::unordered_set<Index>&visited){
        NodeVec nodeVec=adjacentNodes(node.first);

        func(node);
        visited.insert(node.first);

        for(auto &node1:nodeVec){
            if(visited.count(node1.first)==0){
                _DFS_aux(node1,func,visited);
            }
        }
    }

private:
    size_t _size;
    AdjList _nodes;//describe the graph
};
template<class Index,class Value>
class innerIterator:public iterator<bidirectional_iterator_tag,typename graph< Index, Value>::Node>{
public:
    friend class graph<Index,Value>;
    typedef graph<Index,Value> Graph;
    typedef typename vector<typename Graph::Node>::iterator Ptr;

    innerIterator(Graph* g=nullptr,Ptr ptr=Ptr()):_graph(g),_ptr(ptr){

    }
    innerIterator& operator++(){
        ++_ptr;
        return *this;
    }
    innerIterator operator++(int){
        innerIterator tmp = *this;
        ++*this;
        return tmp;
    }
    typename Graph::Node& operator*(){
        //assert(_ptr);
        return *_ptr;
    }

    typename Graph::Node* operator->(){
        return &(operator*());
    }

private:
    Graph* _graph;
    Ptr _ptr;

    //template<class Index,class Value>
    friend bool operator==(const innerIterator<Index,Value>&lhs,const innerIterator<Index,Value>&rhs){
        return lhs._graph==rhs._graph&&lhs._ptr==rhs._ptr;
    }
    //template<class Index,class Value>
    friend bool operator!=(const innerIterator<Index,Value>&lhs,const innerIterator<Index,Value>&rhs){
        return !(lhs==rhs);
    }
};


template<class Index,class Value>
class outerIterator:public iterator<bidirectional_iterator_tag,typename graph< Index, Value>::Node>{
public:
    friend class graph<Index,Value>;
    typedef graph<Index,Value> Graph;
    typedef typename vector<pair<typename Graph::Node, vector< typename Graph::Node> > >::iterator Ptr;

    outerIterator(Graph* g=nullptr,Ptr ptr=Ptr()):_graph(g),_ptr(ptr){

    }
    outerIterator& operator++(){
        ++_ptr;
        return *this;
    }
    outerIterator operator++(int){
        outerIterator tmp = *this;
        ++*this;
        return tmp;
    }
    typename Graph::Node& operator*(){
        return _ptr->first;
    }

    typename Graph::Node* operator->(){
        return &(operator*());
    }

private:
    Graph* _graph;
    Ptr _ptr;

    //template<class Index,class Value>
    friend bool operator==(const outerIterator<Index,Value>&lhs,const outerIterator<Index,Value>&rhs){
        return lhs._graph==rhs._graph&&lhs._ptr==rhs._ptr;
    }
   // template<class Index,class Value>
    friend bool operator!=(const outerIterator<Index,Value>&lhs,const outerIterator<Index,Value>&rhs){
        return !(lhs==rhs);
    }
};

}

#endif
