#include <gtest/gtest.h>
#include "lib/lazy_adapters.h"
#include <vector>
#include <map>
#include <list>
#include <string>

using namespace lazy;

// 1. Filter: retains odd numbers only
TEST(LazyAdaptersTest, FilterOddNumbers) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto result = filter([](int x) { return x % 2 != 0; })(v);
    std::vector<int> expected = {1, 3, 5};
    EXPECT_EQ(result, expected);
}

// 2. Transform: multiply each element by 2
TEST(LazyAdaptersTest, TransformMultiplyByTwo) {
    std::vector<int> v = {1, 2, 3};
    auto result = transform([](int x) { return x * 2; })(v);
    std::vector<int> expected = {2, 4, 6};
    EXPECT_EQ(result, expected);
}

// 3. Take: first N elements
TEST(LazyAdaptersTest, TakeFirstNElements) {
    std::vector<char> v = {'a', 'b', 'c', 'd'};
    auto result = take(3)(v);
    std::vector<char> expected = {'a', 'b', 'c'};
    EXPECT_EQ(result, expected);
}

// 4. Drop: skip first N elements
TEST(LazyAdaptersTest, DropFirstNElements) {
    std::vector<int> v = {10, 20, 30, 40};
    auto result = drop(2)(v);
    std::vector<int> expected = {30, 40};
    EXPECT_EQ(result, expected);
}

// 5. Reverse: reverse the sequence
TEST(LazyAdaptersTest, ReverseSequence) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto result = reverse()(v);
    std::vector<int> expected = {5, 4, 3, 2, 1};
    EXPECT_EQ(result, expected);
}

// 6. Keys: extract keys from associative container
TEST(LazyAdaptersTest, KeysFromMap) {
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
    auto result = keys(m);
    // std::map is ordered by key
    std::vector<std::string> expected = {"one", "three", "two"};
    EXPECT_EQ(result, expected);
}

// 7. Values: extract values from associative container
TEST(LazyAdaptersTest, ValuesFromMap) {
    std::map<std::string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}};
    auto result = values(m);
    std::vector<int> expected = {1, 3, 2};
    EXPECT_EQ(result, expected);
}

// 8. Pipeline: chaining filter and transform
TEST(LazyAdaptersTest, PipelineFilterTransform) {
    std::vector<int> v = {1, 2, 3, 4};
    auto result = v
        | filter([](int x) { return x % 2 == 0; })
        | transform([](int x) { return x + 10; });
    std::vector<int> expected = {12, 14};
    EXPECT_EQ(result, expected);
}

// 9. Adapters on empty container: always return empty
TEST(LazyAdaptersTest, AdaptersOnEmptyContainer) {
    std::vector<int> empty;
    EXPECT_TRUE(filter([](int){ return true; })(empty).empty());
    EXPECT_TRUE(transform([](int x){ return x * x; })(empty).empty());
    EXPECT_TRUE(take(5)(empty).empty());
    EXPECT_TRUE(drop(5)(empty).empty());
    EXPECT_TRUE(reverse()(empty).empty());
    std::map<int,int> m_empty;
    EXPECT_TRUE(keys(m_empty).empty());
    EXPECT_TRUE(values(m_empty).empty());
}

// 10. Adapters on non-random-access container (std::list)
TEST(LazyAdaptersTest, AdaptersOnListContainer) {
    std::list<int> lst = {1, 2, 3, 4, 5};
    auto result = lst
        | drop(1)
        | take(3);
    std::vector<int> expected = {2, 3, 4};
    EXPECT_EQ(result, expected);
}
