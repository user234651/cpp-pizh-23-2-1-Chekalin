// lib/BST_containers.cpp
#include "BST_containers.h"
#include <utility>

// --------------------
// BST_InOrder
// --------------------
template<typename T, typename C, typename A>
BST_InOrder<T,C,A>::BST_InOrder(const C& comp, const A& alloc)
  : root_(nullptr), sz_(0), comp_(comp), alloc_(alloc) {}

template<typename T, typename C, typename A>
BST_InOrder<T,C,A>::~BST_InOrder() {
    clear();
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::Node*
BST_InOrder<T,C,A>::minimum(Node* n) {
    while(n && n->left) n = n->left;
    return n;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::Node*
BST_InOrder<T,C,A>::maximum(Node* n) {
    while(n && n->right) n = n->right;
    return n;
}

template<typename T, typename C, typename A>
void BST_InOrder<T,C,A>::clear_node(Node* n) {
    if(!n) return;
    clear_node(n->left);
    clear_node(n->right);
    std::allocator_traits<NodeAlloc>::destroy(alloc_, n);
    alloc_.deallocate(n,1);
}

template<typename T, typename C, typename A>
void BST_InOrder<T,C,A>::clear() {
    clear_node(root_);
    root_ = nullptr;
    sz_ = 0;
}

template<typename T, typename C, typename A>
std::pair<typename BST_InOrder<T,C,A>::iterator,bool>
BST_InOrder<T,C,A>::insert(const T& v) {
    if(!root_) {
        Node* n = alloc_.allocate(1);
        std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);
        root_ = n;
        ++sz_;
        return {{n,root_}, true};
    }
    Node* cur = root_;            // fixed typo here
    Node* parent = nullptr;
    while(cur) {
        parent = cur;
        if(comp_(v, cur->data))       cur = cur->left;
        else if(comp_(cur->data, v))  cur = cur->right;
        else                          return {{cur, root_}, false};
    }
    Node* n = alloc_.allocate(1);
    std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);
    n->parent = parent;
    if(comp_(v, parent->data)) parent->left = n;
    else                       parent->right = n;
    ++sz_;
    return {{n,root_}, true};
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator
BST_InOrder<T,C,A>::find(const T& v) const {
    Node* cur = root_;
    while(cur) {
        if(comp_(v,cur->data))       cur = cur->left;
        else if(comp_(cur->data,v))  cur = cur->right;
        else                         return {cur,root_};
    }
    return {nullptr,root_};
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::size_type
BST_InOrder<T,C,A>::count(const T& v) const {
    return find(v) != cend();     // use cend() here for const-correctness
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::size_type
BST_InOrder<T,C,A>::size() const {
    return sz_;
}

template<typename T, typename C, typename A>
bool BST_InOrder<T,C,A>::empty() const {
    return sz_ == 0;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator
BST_InOrder<T,C,A>::begin() {
    return { minimum(root_), root_ };
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator
BST_InOrder<T,C,A>::end() {
    return { nullptr, root_ };
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::const_iterator
BST_InOrder<T,C,A>::cbegin() const {
    return { minimum(root_), root_ };
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::const_iterator
BST_InOrder<T,C,A>::cend() const {
    return { nullptr, root_ };
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::reverse_iterator
BST_InOrder<T,C,A>::rbegin() {
    return reverse_iterator(end());
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::reverse_iterator
BST_InOrder<T,C,A>::rend() {
    return reverse_iterator(begin());
}

// iterator
template<typename T, typename C, typename A>
BST_InOrder<T,C,A>::iterator::
iterator(Node* n, Node* root)
  : node_(n), root_(root) {}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator::reference
BST_InOrder<T,C,A>::iterator::operator*() const {
    return node_->data;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator::pointer
BST_InOrder<T,C,A>::iterator::operator->() const {
    return &node_->data;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator&
BST_InOrder<T,C,A>::iterator::operator++() {
    if(node_->right) {
        node_ = minimum(node_->right);
    } else {
        Node* p = node_->parent;
        while(p && node_ == p->right) {
            node_ = p;
            p = p->parent;
        }
        node_ = p;
    }
    return *this;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator
BST_InOrder<T,C,A>::iterator::operator++(int) {
    iterator tmp = *this; ++*this; return tmp;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator&
BST_InOrder<T,C,A>::iterator::operator--() {
    if(!node_) {
        node_ = maximum(root_);
    } else if(node_->left) {
        node_ = maximum(node_->left);
    } else {
        Node* p = node_->parent;
        while(p && node_ == p->left) {
            node_ = p;
            p = p->parent;
        }
        node_ = p;
    }
    return *this;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::iterator
BST_InOrder<T,C,A>::iterator::operator--(int) {
    iterator tmp = *this; --*this; return tmp;
}

template<typename T, typename C, typename A>
bool BST_InOrder<T,C,A>::iterator::operator==(const iterator& o) const {
    return node_ == o.node_;
}

template<typename T, typename C, typename A>
bool BST_InOrder<T,C,A>::iterator::operator!=(const iterator& o) const {
    return node_ != o.node_;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::Node*
BST_InOrder<T,C,A>::iterator::minimum(typename BST_InOrder<T,C,A>::Node* n) {
    while(n->left) n = n->left;
    return n;
}

template<typename T, typename C, typename A>
typename BST_InOrder<T,C,A>::Node*
BST_InOrder<T,C,A>::iterator::maximum(typename BST_InOrder<T,C,A>::Node* n) {
    while(n->right) n = n->right;
    return n;
}

// explicit instantiation
template class BST_InOrder<int>;
template class BST_InOrder<double>;


// --------------------
// BST_PreOrder
// --------------------
template<typename T, typename C, typename A>
BST_PreOrder<T,C,A>::BST_PreOrder(const C& comp, const A& alloc)
  : root_(nullptr), sz_(0), comp_(comp), alloc_(alloc) {}

template<typename T, typename C, typename A>
BST_PreOrder<T,C,A>::~BST_PreOrder() {
    clear();
}

template<typename T, typename C, typename A>
void BST_PreOrder<T,C,A>::clear_node(typename BST_PreOrder<T,C,A>::Node* n) {
    if(!n) return;
    clear_node(n->left);
    clear_node(n->right);
    std::allocator_traits<typename BST_PreOrder<T,C,A>::NodeAlloc>::destroy(alloc_, n);
    alloc_.deallocate(n,1);
}

template<typename T, typename C, typename A>
void BST_PreOrder<T,C,A>::clear() {
    clear_node(root_);
    root_ = nullptr;
    sz_   = 0;
}

template<typename T, typename C, typename A>
std::pair<typename BST_PreOrder<T,C,A>::iterator,bool>
BST_PreOrder<T,C,A>::insert(const T& v) {
    if(!root_) {
        Node* n = alloc_.allocate(1);
        std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);
        root_ = n; ++sz_;
        return {{n},true};
    }
    Node* cur = root_;
    Node* p   = nullptr;
    while(cur) {
        p = cur;
        if(comp_(v,cur->data))       cur = cur->left;
        else if(comp_(cur->data,v))  cur = cur->right;
        else                          return {{cur},false};
    }
    Node* n = alloc_.allocate(1);
    std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);
    n->parent = p;
    if(comp_(v,p->data)) p->left = n;
    else                 p->right = n;
    ++sz_;
    return {{n},true};
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator
BST_PreOrder<T,C,A>::find(const T& v) const {
    Node* cur = root_;
    while(cur) {
        if(comp_(v,cur->data))       cur = cur->left;
        else if(comp_(cur->data,v))  cur = cur->right;
        else                         return {cur};
    }
    return {nullptr};
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::size_type
BST_PreOrder<T,C,A>::count(const T& v) const {
    return find(v) != cend();   // use cend() here
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::size_type
BST_PreOrder<T,C,A>::size() const {
    return sz_;
}

template<typename T, typename C, typename A>
bool BST_PreOrder<T,C,A>::empty() const {
    return sz_ == 0;
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator
BST_PreOrder<T,C,A>::begin() {
    return { root_ };
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator
BST_PreOrder<T,C,A>::end() {
    return { nullptr };
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::const_iterator
BST_PreOrder<T,C,A>::cbegin() const {
    return { root_ };
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::const_iterator
BST_PreOrder<T,C,A>::cend() const {
    return { nullptr };
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::reverse_iterator
BST_PreOrder<T,C,A>::rbegin() {
    return reverse_iterator(end());
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::reverse_iterator
BST_PreOrder<T,C,A>::rend() {
    return reverse_iterator(begin());
}

// iterator
template<typename T, typename C, typename A>
BST_PreOrder<T,C,A>::iterator::
iterator(Node* n) : node_(n) {}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator::reference
BST_PreOrder<T,C,A>::iterator::operator*() const {
    return node_->data;
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator::pointer
BST_PreOrder<T,C,A>::iterator::operator->() const {
    return &node_->data;
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator&
BST_PreOrder<T,C,A>::iterator::operator++() {
    node_ = next_pre(node_);
    return *this;
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator
BST_PreOrder<T,C,A>::iterator::operator++(int) {
    iterator tmp = *this; ++*this; return tmp;
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator&
BST_PreOrder<T,C,A>::iterator::operator--() {
    node_ = prev_pre(node_);
    return *this;
}

template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::iterator
BST_PreOrder<T,C,A>::iterator::operator--(int) {
    iterator tmp = *this; --*this; return tmp;
}

template<typename T, typename C, typename A>
bool BST_PreOrder<T,C,A>::iterator::operator==(const iterator& o) const {
    return node_ == o.node_;
}

template<typename T, typename C, typename A>
bool BST_PreOrder<T,C,A>::iterator::operator!=(const iterator& o) const {
    return node_ != o.node_;
}

// pre-order next
template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::Node*
BST_PreOrder<T,C,A>::iterator::next_pre(Node* n) {
    if(!n) return nullptr;
    if(n->left) return n->left;
    if(n->right) return n->right;
    while(n->parent) {
        if(n == n->parent->left && n->parent->right)
            return n->parent->right;
        n = n->parent;
    }
    return nullptr;
}

// pre-order prev (optional full implementation omitted for brevity)
template<typename T, typename C, typename A>
typename BST_PreOrder<T,C,A>::Node*
BST_PreOrder<T,C,A>::iterator::prev_pre(Node* n) {
    return nullptr;
}

// explicit instantiation
template class BST_PreOrder<int>;
template class BST_PreOrder<double>;


// --------------------
// BST_PostOrder
// --------------------
template<typename T, typename C, typename A>
BST_PostOrder<T,C,A>::BST_PostOrder(const C& comp, const A& alloc)
  : root_(nullptr), sz_(0), comp_(comp), alloc_(alloc) {}

template<typename T, typename C, typename A>
BST_PostOrder<T,C,A>::~BST_PostOrder() {
    clear();
}

template<typename T, typename C, typename A>
void BST_PostOrder<T,C,A>::clear_node(typename BST_PostOrder<T,C,A>::Node* n) {
    if(!n) return;
    clear_node(n->left);
    clear_node(n->right);
    std::allocator_traits<typename BST_PostOrder<T,C,A>::NodeAlloc>::destroy(alloc_, n);
    alloc_.deallocate(n,1);
}

template<typename T, typename C, typename A>
void BST_PostOrder<T,C,A>::clear() {
    clear_node(root_);
    root_ = nullptr;
    sz_   = 0;
}

template<typename T, typename C, typename A>
std::pair<typename BST_PostOrder<T,C,A>::iterator,bool>
BST_PostOrder<T,C,A>::insert(const T& v) {
    if(!root_) {
        Node* n = alloc_.allocate(1);
        std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);
        root_ = n; ++sz_;
        return {{n,root_},true};
    }
    Node* cur = root_;
    Node* p   = nullptr;
    while(cur) {
        p = cur;
        if(comp_(v,cur->data))       cur = cur->left;
        else if(comp_(cur->data,v))  cur = cur->right;
        else                          return {{cur,root_},false};
    }
    Node* n = alloc_.allocate(1);
    std::allocator_traits<NodeAlloc>::construct(alloc_, n, v);
    n->parent = p;
    if(comp_(v,p->data)) p->left  = n;
    else                 p->right = n;
    ++sz_;
    return {{n,root_},true};
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator
BST_PostOrder<T,C,A>::find(const T& v) const {
    Node* cur = root_;
    while(cur) {
        if(comp_(v,cur->data))       cur = cur->left;
        else if(comp_(cur->data,v))  cur = cur->right;
        else                         return {cur,root_};
    }
    return {nullptr,root_};
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::size_type
BST_PostOrder<T,C,A>::count(const T& v) const {
    return find(v) != cend();   // use cend() here
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::size_type
BST_PostOrder<T,C,A>::size() const {
    return sz_;
}

template<typename T, typename C, typename A>
bool BST_PostOrder<T,C,A>::empty() const {
    return sz_ == 0;
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator
BST_PostOrder<T,C,A>::begin() {
    return { iterator::first_post(root_), root_ };  // qualify first_post
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator
BST_PostOrder<T,C,A>::end() {
    return { nullptr, root_ };
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::const_iterator
BST_PostOrder<T,C,A>::cbegin() const {
    return { iterator::first_post(root_), root_ };  // qualify first_post
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::const_iterator
BST_PostOrder<T,C,A>::cend() const {
    return { nullptr, root_ };
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::reverse_iterator
BST_PostOrder<T,C,A>::rbegin() {
    return reverse_iterator(end());
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::reverse_iterator
BST_PostOrder<T,C,A>::rend() {
    return reverse_iterator(begin());
}

// iterator
template<typename T, typename C, typename A>
BST_PostOrder<T,C,A>::iterator::
iterator(Node* n, Node* root)
  : node_(n), root_(root) {}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator::reference
BST_PostOrder<T,C,A>::iterator::operator*() const {
    return node_->data;
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator::pointer
BST_PostOrder<T,C,A>::iterator::operator->() const {
    return &node_->data;
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator&
BST_PostOrder<T,C,A>::iterator::operator++() {
    node_ = next_post(node_, root_);
    return *this;
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator
BST_PostOrder<T,C,A>::iterator::operator++(int) {
    iterator tmp = *this; ++*this; return tmp;
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator&
BST_PostOrder<T,C,A>::iterator::operator--() {
    if(!node_) node_ = first_post(root_);
    else       node_ = prev_post(node_, root_);
    return *this;
}

template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::iterator
BST_PostOrder<T,C,A>::iterator::operator--(int) {
    iterator tmp = *this; --*this; return tmp;
}

template<typename T, typename C, typename A>
bool BST_PostOrder<T,C,A>::iterator::operator==(const iterator& o) const {
    return node_ == o.node_;
}

template<typename T, typename C, typename A>
bool BST_PostOrder<T,C,A>::iterator::operator!=(const iterator& o) const {
    return node_ != o.node_;
}

// find first node in post‐order
template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::Node*
BST_PostOrder<T,C,A>::iterator::first_post(Node* n) {
    if(!n) return nullptr;
    while(n->left || n->right) {
        if(n->left)  n = n->left;
        else         n = n->right;
    }
    return n;
}

// next in post‐order
template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::Node*
BST_PostOrder<T,C,A>::iterator::next_post(Node* cur, Node* root) {
    if(!cur) return nullptr;
    if(cur == root) return nullptr;
    Node* p = cur->parent;
    if(p->right && cur != p->right) {
        cur = p->right;
        while(cur->left || cur->right) {
            if(cur->left)  cur = cur->left;
            else           cur = cur->right;
        }
        return cur;
    }
    return p;
}

// prev in post‐order
template<typename T, typename C, typename A>
typename BST_PostOrder<T,C,A>::Node*
BST_PostOrder<T,C,A>::iterator::prev_post(Node* cur, Node* root) {
    if(!cur) return root;
    if(cur->right) {
        cur = cur->right;
        while(cur->left || cur->right) {
            if(cur->right) cur = cur->right;
            else           cur = cur->left;
        }
        return cur;
    }
    if(cur->left) {
        cur = cur->left;
        while(cur->left || cur->right) {
            if(cur->right) cur = cur->right;
            else           cur = cur->left;
        }
        return cur;
    }
    return cur->parent;
}

template class BST_PostOrder<int>;
template class BST_PostOrder<double>;
