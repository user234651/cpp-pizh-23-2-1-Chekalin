// bin/main.cpp
#include <iostream>
#include "lib/BST_containers.h"

int main() {
    BST_InOrder<int>   bst_in;
    BST_PreOrder<int>  bst_pre;
    BST_PostOrder<int> bst_post;

    int a[] = { 5, 2, 8, 1, 3, 7, 9 };
    for(int v: a) {
        bst_in.insert(v);
        bst_pre.insert(v);
        bst_post.insert(v);
    }

    std::cout << "In-order: ";
    for(auto it = bst_in.begin(); it != bst_in.end(); ++it)
        std::cout << *it << ' ';
    std::cout << "\n";

    std::cout << "Pre-order: ";
    for(auto it = bst_pre.begin(); it != bst_pre.end(); ++it)
        std::cout << *it << ' ';
    std::cout << "\n";

    std::cout << "Post-order: ";
    for(auto it = bst_post.begin(); it != bst_post.end(); ++it)
        std::cout << *it << ' ';
    std::cout << "\n";

    // reverse iteration
    std::cout << "In-order reversed: ";
    for(auto it = bst_in.rbegin(); it != bst_in.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << "\n";

    return 0;
}
