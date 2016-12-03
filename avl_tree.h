#ifndef JIN_AVL_TREE_H_
#define JIN_AVL_TREE_H_

#include "alloc.h"
#include "iterator.h"
#include "stack.h"
//#include<queue>
//using std::queue;
#include"queue.h"
#include"vector.h"
#include<set>
#include<string>
using std::string;

namespace jinstl{

        template<class T>
        class avl_tree;

        template<class T>
        struct avl_node{
                T _data;
                avl_node *_left, *_right;
                size_t _height;
                typedef T value_type;
                explicit avl_node(T d = T(), avl_node *l = 0, avl_node *r = 0, size_t h = 1)
                        :_data(d), _left(l), _right(r), _height(h){}
        };


        //class of avl tree iterator
        template<class T>//
        class avl_iterator {
        private:

                friend class avl_tree<T>;
        public:
                typedef forward_iterator_tag iterator_category;
                typedef T value_type;
                typedef typename avl_tree<T>::const_reference const_reference;
                typedef const avl_node<T>* const_pointer;
        private:
                const avl_node<T>* _ptr;
                const avl_tree<T>* _container;
                stack<const avl_node<T>*> _parent;
                std::set<const avl_node<T>*> _visited;
        public:
                avl_iterator(const avl_node<T>*ptr, const avl_tree<T>* container)
                    :_ptr(ptr),_container(container)
                {
                    if(!_ptr) return;
                    avl_node<T>* tmp = container->_root;
                    while(tmp&&_ptr!=tmp&&_ptr->_data!=tmp->_data){
                        _parent.push(tmp);
                        if(tmp->_data<_ptr->_data){
                            _visited.insert(tmp);
                            tmp=tmp->_right;
                        }else if(tmp->_data>_ptr->_data){
                            tmp=tmp->_left;
                        }
                    }
                }

                operator const avl_node<T>*()const{ return _ptr; }
                const_reference operator*()const{ return _ptr->_data; }
                const_pointer operator ->()const{ return &_ptr->_data; }

                avl_iterator left()const{ return avl_iterator(_ptr->_left, _container); }
                avl_iterator right()const{ return avl_iterator(_ptr->_right, _container); }
                //move iterator to next bigger node
                avl_iterator& operator ++(){
                    _visited.insert(_ptr);
                    if(_ptr->_right){
                        _parent.push(_ptr);
                        _ptr=_ptr->_right;
                        while(_ptr&&_ptr->_left){
                            _parent.push(_ptr);
                            _ptr=_ptr->_left;
                        }
                    }else{//go to father node
                        while(!_parent.empty()){
                            _ptr=_parent.top();
                            _parent.pop();
                            if(_visited.count(_ptr)==0){
                                _visited.insert(_ptr);
                                break;
                            }
                        }
                        _ptr=0;//means no bigger else
                    }
                    return *this;
                }

                avl_iterator operator ++(int){
                    avl_iterator res = *this;
                    ++*this;
                    return res;
                }

        public:
                //template<class T>
                friend bool operator ==(const avl_iterator<T>& it1, const avl_iterator<T>& it2){
                    return it1._ptr==it2._ptr;
                }

                //template<class T>
                friend bool operator !=(const avl_iterator<T>& it1, const avl_iterator<T>& it2){
                    return !(it1==it2);
                }
        };//end of avl_iterator



	//class of avl tree
        template<class T>
	class avl_tree{
	private:
                //template<class T>
                friend class avl_iterator<T>;
	private:

                typedef jinstl::simple_alloc<avl_node<T>,alloc> data_allocator;
	public:
		typedef T value_type;
                typedef avl_iterator<T> const_iterator;
		typedef const T& const_reference;
		typedef T& reference;
		typedef size_t size_type;
	private:
                avl_node<T> *_root;
                size_type _size;
	public:
                avl_tree() :_root(0), _size(0){}
		avl_tree(const avl_tree&) = delete;
		avl_tree& operator = (const avl_tree&) = delete;
                ~avl_tree(){
                    destroyAndDeallocateAllNodes(_root);
                }

                void insert(const T& val){
                    _insert_aux(val,_root);
                }

                template<class InputIterator>
                void insert(InputIterator first, InputIterator last){
                    for(;first!=last;++first){
                        insert(*first);
                    }
                }

                void erase(const T& val){
                    _erase_aux(val,_root);
                }

                size_t height()const{ return _getHeight(_root); }
                size_t size()const{ return _size; }
		bool empty()const{ return _root == 0; }
		const_iterator root()const{ return const_iterator(_root, this); }

		const_iterator cbegin()const{ return find_min(); }
		const_iterator cend()const{ return const_iterator(0, this); }

                const_iterator find_min()const{
                    avl_node<T>* tmp = _root;
                    while(tmp&&tmp->_left){
                        tmp = tmp->_left;
                    }
                    return const_iterator(tmp,this);

                }
                const_iterator find_max()const{
                    avl_node<T>* tmp = _root;
                    while(tmp&&tmp->_right){
                        tmp = tmp->_right;
                    }
                    return const_iterator(tmp,this);
                }
                const_iterator find(const T& val)const{
                    avl_node<T>* tmp = _root;
                    while(tmp){
                        if(tmp->_data==val) return const_iterator(tmp,this);
                        else if(tmp->_data>val){
                            tmp=tmp->_left;
                        }else{
                            tmp=tmp->_right;
                        }
                    }
                    return const_iterator(0,this);
                }

                void print_preorder(const string& delim = " ", std::ostream& os = std::cout)const{
                    _print_preorder_aux(delim,os,_root);
                }
                void print_inorder(const string& delim = " ", std::ostream& os = std::cout)const{
                    _print_inorder_aux(delim,os,_root);
                }
                void print_postorder(const string& delim = " ", std::ostream& os = std::cout)const{
                    _print_postorder_aux(delim,os,_root);
                }
                void print_levelorder(const string& delim = " ", std::ostream& os = std::cout)const{
                    _print_levelorder_aux(delim,os,_root);
                }
	private:
                avl_node<T> *singleLeftLeftRotate(avl_node<T> *k2){
                    avl_node<T>* k1=k2->_left;
                    avl_node<T>* k1right=k1->_right;
                    k2->_left=k1right;
                    k1->_right=k2;

                    k2->_height=jinstl::max(_getHeight(k1->_left),_getHeight(k2->_right))+1;
                    k1->_height=jinstl::max(_getHeight(k1->_left),_getHeight(k2))+1;
                    return k1;
                }
                avl_node<T> *singleRightRightRotate(avl_node<T> * k2){
                    avl_node<T>* k1=k2->_right;
                    avl_node<T>* k1left=k1->_left;
                    k2->_right=k1left;
                    k1->_left=k2;
                    k2->_height=jinstl::max(_getHeight(k1->_right),_getHeight(k2->_left))+1;
                    k1->_height=jinstl::max(_getHeight(k1->_right),_getHeight(k2))+1;

                    return k1;
                }
                avl_node<T> *doubleLeftRightRotate(avl_node<T> * k3){
                    avl_node<T> *k1=k3->_left,*k2;
                    k3->_left=singleRightRightRotate(k1);
                    k2 = singleLeftLeftRotate(k3);
                    return k2;
                }

                avl_node<T> *doubleRightLeftRotate(avl_node<T> * k3){
                    avl_node<T> *k1=k3->_right,*k2;
                    k3->_right=singleLeftLeftRotate(k1);
                    k2 = singleRightRightRotate(k3);
                    return k2;
                }



	private:

                void destroyAndDeallocateAllNodes(avl_node<T> *p){
                    if(p!=0){
                        destroyAndDeallocateAllNodes(p->_left);
                        destroyAndDeallocateAllNodes(p->_right);
                        destroy(p);
                        data_allocator::deallocate(p);
                    }
                }

                size_t _getHeight(const avl_node<T> *p)const{
                    return p == 0 ? 0 : p->_height;
                }

                void _insert_aux(const T& val, avl_node<T> *&p){
                        if (p == 0){
                                p = data_allocator::allocate(sizeof(avl_node<T>));
                                construct(p,avl_node<T>());
                                p->_data = val;
                                p->_left = p->_right = 0;
                                p->_height = 1;
                                ++_size;
                        }
                        else if (p->_data < val){
                                _insert_aux(val, p->_right);
                                if (_getHeight(p->_right) - _getHeight(p->_left) == 2){
                                        if (val > p->_right->_data)
                                                p = singleRightRightRotate(p);
                                        else
                                                p = doubleRightLeftRotate(p);
                                }
                        }
                        else if (p->_data > val){
                                _insert_aux(val, p->_left);
                                if (_getHeight(p->_left) - _getHeight(p->_right) == 2){
                                        if (val < p->_left->_data)
                                                p = singleLeftLeftRotate(p);
                                        else
                                                p = doubleLeftRightRotate(p);
                                }
                        }
                        p->_height = max(_getHeight(p->_left), _getHeight(p->_right)) + 1;
                }


                void _erase_aux(const T& val, avl_node<T> *&p){
                        if (p == 0)
                                return;
                        if (p->_data != val){
                                if (val < p->_data)
                                        return _erase_aux(val, p->_left);
                                else
                                        return _erase_aux(val, p->_right);
                        }else{ // found
                                if (p->_left != 0 && p->_right != 0){// has two children
                                        size_t choose = _size % 2;//random choose left or right


                                        avl_node<T> * pos = (choose == 0 ?
                                                const_cast<avl_node<T> *>(_find_min_aux(p->_right)._ptr) : const_cast<avl_node<T> *>(_find_max_aux(p->_left)._ptr));
                                        p->_data = pos->_data;
                                        return (choose == 0 ? _erase_aux(pos->_data, p->_right) : _erase_aux(pos->_data, p->_left));
                                }else{ //has one or no child
                                        avl_node<T> * temp = p;
                                        if (p->_left == 0)
                                                p = p->_right;
                                        else
                                                p = p->_left;

                                        data_allocator::deallocate(temp);
                                        --_size;
                                }
                        }
                        if (p != 0){//update p height,consider the "last else" above.
                                p->_height = max(_getHeight(p->_left), _getHeight(p->_right)) + 1;
                                if (_getHeight(p->_left) - _getHeight(p->_right) == 2){
                                        if (p->_left->_right == 0)
                                                singleLeftLeftRotate(p);
                                        else
                                                doubleLeftRightRotate(p);
                                }else if (_getHeight(p->_right) - _getHeight(p->_left) == 2){
                                        if (p->_right->_left == 0)
                                                singleRightRightRotate(p);
                                        else
                                                doubleRightLeftRotate(p);
                                }
                        }
                }

                const_iterator _find_min_aux(const avl_node<T>* root)const{

                    while(root&&root->_left){
                        root = root->_left;
                    }
                    return const_iterator(root,this);

                }
                const_iterator _find_max_aux(const avl_node<T>* root)const{
                    //avl_node<T>* tmp = root;
                    while(root&&root->_right){
                        root = root->_right;
                    }
                    return const_iterator(root,this);
                }


                void _print_preorder_aux(const string& delim = " ", std::ostream& os = std::cout,avl_node<T>* root=0)const{
                    /*if(root==0) return;
                    os<<root->_data<<delim;
                    _print_preorder_aux(delim,os,root->_left);
                    _print_preorder_aux(delim,os,root->_right);*/
                    stack<avl_node<T>*>s;
                    if(root==0) return;
                    s.push(root);
                    while(!s.empty()){
                        root=s.top();s.pop();
                        os<<root->_data<<delim;
                        if(root->_right) s.push(root->_right);
                        if(root->_left) s.push(root->_left);
                    }


                }
                void _print_inorder_aux(const string& delim = " ", std::ostream& os = std::cout,avl_node<T>* root=0)const{
                    /*if(root==0) return;
                    _print_inorder_aux(delim,os,root->_left);
                    os<<root->_data<<delim;
                    _print_inorder_aux(delim,os,root->_right);*/

                    stack<avl_node<T>*> s;
                    while(root||!s.empty()){
                        while(root){
                            s.push(root);
                            root=root->_left;
                        }

                        root=s.top();s.pop();
                        os<<root->_data<<delim;
                        root=root->_right;
                    }
                }
                void _print_postorder_aux(const string& delim = " ", std::ostream& os = std::cout,avl_node<T>* root=0)const{
                   /* if(root==0) return;
                    _print_postorder_aux(delim,os,root->_left);
                    _print_postorder_aux(delim,os,root->_right);
                    os<<root->_data<<delim;*/
                    stack<avl_node<T>*>s;
                    vector<avl_node<T>*> v;
                    if(root==0) return;
                    s.push(root);
                    while(!s.empty()){
                        root=s.top();s.pop();
                        v.push_back(root);
                        if(root->_left) s.push(root->_left);
                        if(root->_right) s.push(root->_right);

                    }
                    //reverse
                    typename vector<avl_node<T>*>::iterator it=v.begin();//need typename!!
                    while(it!=v.end()){
                        s.push(*it);
                        it++;
                    }
                    while(!s.empty()){
                        os<<s.top()->_data<<delim;
                        s.pop();
                    }
                }
                void _print_levelorder_aux(const string& delim = " ", std::ostream& os = std::cout,avl_node<T>* root=0)const{
                    if(root==0) return;
                    queue<avl_node<T>*> q;
                    q.push(root);
                    while(!q.empty()){
                        avl_node<T>* tmp=q.front();
                        if(tmp->_left) q.push(tmp->_left);
                        if(tmp->_right) q.push(tmp->_right);
                        os<<tmp->_data<<delim;
                        q.pop();
                    }
                }
        };

}



#endif
