#include"avl_tree.h"
#include"vector.h"
#include<iostream>
using namespace jinstl;
int main(){
    avl_tree<int> tree;
    int a[]={1,2,3,4,5,6,7};
    tree.insert(a,a+7);
    tree.print_levelorder();std::cout<<std::endl;
    tree.print_inorder();std::cout<<std::endl;
    tree.print_preorder();std::cout<<std::endl;
    tree.print_postorder();std::cout<<std::endl;
    //tree.erase(7);
}
