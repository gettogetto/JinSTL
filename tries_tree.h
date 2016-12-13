#ifndef JIN_TRIE_H
#define JIN_TRIE_H
#include<unordered_map>
#include<vector>
#include<string>
#include"memory.h"
using std::unordered_map;
using std::vector;
using std::string;
namespace jinstl{
class TrieNode{
public:
    char _data;
    bool _isword;
    unordered_map<char,unique_ptr<TrieNode> > _childs;

    TrieNode():_data('\0'),_isword(0){}
    TrieNode(char data,bool isword):_data(data),_isword(isword){}

};

class TrieTree{
private:
    unique_ptr<TrieNode> _root;
    size_t _size;
private:
    bool _addword_aux(unique_ptr <TrieNode>& root,const string& word){
        if(word.empty()){
            root->_isword=true;
            ++_size;
            return true;
        }
        char ch=word[0];
        auto it=root->_childs.find(ch);
        if(it==root->_childs.end()){
            auto node=makenode(ch,word.size()==1?true:false);
            root->_childs[ch]=std::move(node);
            _addword_aux(root->_childs[ch],word.substr(1));
            return true;
        }else{

            return _addword_aux(root->_childs[ch],word.substr(1));
        }
    }
    //the function is used when word[0] is already in root!!
    bool _existed_aux(unique_ptr <TrieNode>& root,const string& word){
        if(word.size()==1) return root->_isword;
        auto it=root->_childs.find(word[1]);
        if(it==root->_childs.end()) return false;
        else{
            return _existed_aux(it->second,word.substr(1));
        }
    }

    //prefix[0] is already in node root
    void _getword_aux(unique_ptr<TrieNode>&root,const string& prefix,const string& originprefix,vector<string>&words){
        if(prefix.size()==1){
            if(root->_isword){
                words.push_back(originprefix);
            }
            for(auto it=root->_childs.begin();it!=root->_childs.end();it++){
                string str;
                _getword_aux_helper(it->second,str,originprefix,words);
            }
        }else{
            auto it=root->_childs.find(prefix[1]);
            if(it==root->_childs.end()){
                return;
            }else{
                _getword_aux(it->second,prefix.substr(1),originprefix,words);
            }
        }


    }
    void _getword_aux_helper(unique_ptr<TrieNode>&root, string& word,const string& originprefix,vector<string>&words){
        word+=root->_data;
        if(root->_isword){
            words.push_back(originprefix+word);
        }
        for(auto it=root->_childs.begin();it!=root->_childs.end();it++){
            string str=word;
            _getword_aux_helper(it->second,str,originprefix,words);
        }
    }

public:
    TrieTree()
        :_root(new TrieNode),_size(0){

    }
    ~TrieTree(){}
    TrieTree(const TrieTree&)=delete;
    TrieTree& operator=(const TrieTree&)=delete;

    size_t size(){return _size;}
    bool empty(){return size()==0;}
    unique_ptr<TrieNode> makenode(char ch,bool isword){
        return make_unique<TrieNode>(ch,isword);
    }

    vector<string> getword(const string& prefix){
        vector<string> res;
        if(_root==nullptr) return res;
        if(prefix.empty()) return res;

        auto it=_root->_childs.find(prefix[0]);
        if(it!=_root->_childs.end()){
            _getword_aux(it->second,prefix,prefix,res);

        }
        return res;
    }
    bool addword(const string& word){
        if(existed(word)) return true;
        if(word.empty()) return false;

        return _addword_aux(_root,word);
    }
    bool existed(const string& word){
        if(word.empty()) return false;
        auto it=_root->_childs.find(word[0]);
        if(it==_root->_childs.end()) return false;
        else{
            return _existed_aux(it->second,word);
        }
    }


};

}

#endif
