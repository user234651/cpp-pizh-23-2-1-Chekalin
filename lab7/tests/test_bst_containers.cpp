#include <gtest/gtest.h>
#include "lib/BST_containers.h"
#include <type_traits>
#include <vector>
#include <algorithm>

// Helper to collect sequence from iterators
template<typename Iter>
std::vector<typename std::iterator_traits<Iter>::value_type> to_vector(Iter first, Iter last) {
    std::vector<typename std::iterator_traits<Iter>::value_type> v;
    for (; first != last; ++first) v.push_back(*first);
    return v;
}

// In-order BST tests
TEST(BST_InOrder, TypeRequirements) {
    using C = BST_InOrder<int>;
    static_assert(std::is_copy_constructible<C>::value, "Copy constructible");
    static_assert(std::is_copy_assignable<C>::value,   "Copy assignable");
    static_assert(std::is_destructible<C>::value,      "Destructible");
    static_assert(std::is_same<typename C::value_type, int>::value, "value_type");
    static_assert(std::is_same<typename C::key_compare, std::less<int>>::value, "key_compare");
    static_assert(std::is_same<typename C::allocator_type, std::allocator<int>>::value, "allocator_type");
    using It = typename C::iterator;
    using Cat = typename std::iterator_traits<It>::iterator_category;
    static_assert(std::is_same<Cat, std::bidirectional_iterator_tag>::value, "BidirectionalIterator");
}

TEST(BST_InOrder, BasicOperations) {
    BST_InOrder<int> bst;
    EXPECT_TRUE(bst.empty());
    EXPECT_EQ(bst.size(), 0u);
    EXPECT_EQ(bst.count(42), 0u);
    EXPECT_EQ(bst.find(42), bst.end());
    std::vector<int> data = {4,1,3,2};
    for(int v: data) EXPECT_TRUE(bst.insert(v).second);
    EXPECT_EQ(bst.size(), data.size());
    EXPECT_FALSE(bst.empty());
    // in-order traversal
    auto seq = to_vector(bst.begin(), bst.end());
    std::vector<int> expected = {1,2,3,4};
    EXPECT_EQ(seq, expected);
    // clear
    bst.clear();
    EXPECT_TRUE(bst.empty());
    EXPECT_EQ(bst.begin(), bst.end());
}

TEST(BST_InOrder, ConstIteratorTraversal) {
    BST_InOrder<int> bst;
    for(int v: {10,5,15}) bst.insert(v);
    auto seq = to_vector(bst.cbegin(), bst.cend());
    std::vector<int> expected = {5,10,15};
    EXPECT_EQ(seq, expected);
}

// Pre-order BST tests
TEST(BST_PreOrder, Traversal) {
    BST_PreOrder<int> bst;
    for(int v: {5,2,8,1,3,7,9}) bst.insert(v);
    std::vector<int> expected = {5,2,1,3,8,7,9};
    auto seq = to_vector(bst.begin(), bst.end());
    EXPECT_EQ(seq, expected);
}

TEST(BST_PreOrder, FindAndCountMissing) {
    BST_PreOrder<int> bst;
    for(int v: {1,2,3}) bst.insert(v);
    EXPECT_EQ(bst.find(42), bst.end());
    EXPECT_EQ(bst.count(42), 0u);
}

// Post-order BST tests
TEST(BST_PostOrder, Traversal) {
    BST_PostOrder<int> bst;
    for(int v: {5,2,8,1,3,7,9}) bst.insert(v);
    std::vector<int> expected = {1,3,2,7,9,8,5};
    auto seq = to_vector(bst.begin(), bst.end());
    EXPECT_EQ(seq, expected);
}

TEST(BST_PostOrder, EmptyContainer) {
    BST_PostOrder<double> bst;
    EXPECT_TRUE(bst.empty());
    EXPECT_EQ(bst.begin(), bst.end());
}