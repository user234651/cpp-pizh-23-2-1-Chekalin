#pragma once
#include <memory>
#include <functional>
#include <iterator>
#include <utility>
#include <cstddef>

//
// In-order BST
//
template<typename T,
         typename Compare   = std::less<T>,
         typename Allocator = std::allocator<T>>
class BST_InOrder {
public:
    using value_type      = T;
    using key_compare     = Compare;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;

    class iterator;
    using const_iterator         = iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;

    BST_InOrder(const Compare& comp = Compare(),
                const Allocator& alloc = Allocator());
    ~BST_InOrder();

    std::pair<iterator,bool> insert(const T& v);
    iterator                find(const T& v) const;
    size_type               count(const T& v) const;
    void                    clear();
    size_type               size() const;
    bool                    empty() const;

    iterator                begin();
    iterator                end();
    const_iterator          cbegin() const;
    const_iterator          cend() const;
    reverse_iterator        rbegin();
    reverse_iterator        rend();

private:
    struct Node {
        T    data;
        Node* left;
        Node* right;
        Node* parent;
        Node(const T& v): data(v), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    Node*      root_;
    size_type  sz_;
    Compare    comp_;
    NodeAlloc  alloc_;

    static Node* minimum(Node* n);
    static Node* maximum(Node* n);
    void clear_node(Node*);

public:
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator(Node* n = nullptr, Node* root = nullptr);

        reference   operator* () const;
        pointer     operator->() const;
        iterator&   operator++();
        iterator    operator++(int);
        iterator&   operator--();
        iterator    operator--(int);
        bool        operator==(const iterator& o) const;
        bool        operator!=(const iterator& o) const;

        // allow qualification minimum/maximum from outside
        static Node* minimum(Node* n);
        static Node* maximum(Node* n);

    private:
        Node* node_;
        Node* root_;
    };
};

//
// Pre-order BST
//
template<typename T,
         typename Compare   = std::less<T>,
         typename Allocator = std::allocator<T>>
class BST_PreOrder {
public:
    using value_type      = T;
    using key_compare     = Compare;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;

    class iterator;
    using const_iterator         = iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;

    BST_PreOrder(const Compare& comp = Compare(),
                 const Allocator& alloc = Allocator());
    ~BST_PreOrder();

    std::pair<iterator,bool> insert(const T& v);
    iterator                find(const T& v) const;
    size_type               count(const T& v) const;
    void                    clear();
    size_type               size() const;
    bool                    empty() const;

    iterator                begin();
    iterator                end();
    const_iterator          cbegin() const;
    const_iterator          cend() const;
    reverse_iterator        rbegin();
    reverse_iterator        rend();

private:
    struct Node {
        T    data;
        Node* left;
        Node* right;
        Node* parent;
        Node(const T& v): data(v), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    Node*      root_;
    size_type  sz_;
    Compare    comp_;
    NodeAlloc  alloc_;

    void clear_node(Node*);

public:
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator(Node* n = nullptr);

        reference   operator* () const;
        pointer     operator->() const;
        iterator&   operator++();
        iterator    operator++(int);
        iterator&   operator--();
        iterator    operator--(int);
        bool        operator==(const iterator& o) const;
        bool        operator!=(const iterator& o) const;

    private:
        Node* node_;

        static Node* next_pre(Node*);
        static Node* prev_pre(Node*);
    };
};

//
// Post-order BST
//
template<typename T,
         typename Compare   = std::less<T>,
         typename Allocator = std::allocator<T>>
class BST_PostOrder {
public:
    using value_type      = T;
    using key_compare     = Compare;
    using allocator_type  = Allocator;
    using size_type       = std::size_t;

    class iterator;
    using const_iterator         = iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;

    BST_PostOrder(const Compare& comp = Compare(),
                  const Allocator& alloc = Allocator());
    ~BST_PostOrder();

    std::pair<iterator,bool> insert(const T& v);
    iterator                find(const T& v) const;
    size_type               count(const T& v) const;
    void                    clear();
    size_type               size() const;
    bool                    empty() const;

    iterator                begin();
    iterator                end();
    const_iterator          cbegin() const;
    const_iterator          cend() const;
    reverse_iterator        rbegin();
    reverse_iterator        rend();

private:
    struct Node {
        T    data;
        Node* left;
        Node* right;
        Node* parent;
        Node(const T& v): data(v), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    Node*      root_;
    size_type  sz_;
    Compare    comp_;
    NodeAlloc  alloc_;

    void clear_node(Node*);

public:
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        iterator(Node* n = nullptr, Node* root = nullptr);

        reference   operator* () const;
        pointer     operator->() const;
        iterator&   operator++();
        iterator    operator++(int);
        iterator&   operator--();
        iterator    operator--(int);
        bool        operator==(const iterator& o) const;
        bool        operator!=(const iterator& o) const;

        // Сделали public, чтобы BST_PostOrder::begin()/cbegin() могли вызвать
        static Node* first_post(Node* n);

    private:
        Node* node_;
        Node* root_;

        static Node* next_post(Node*, Node*);
        static Node* prev_post(Node*, Node*);
    };
};
