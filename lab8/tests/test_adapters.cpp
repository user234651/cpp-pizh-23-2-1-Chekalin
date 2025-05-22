#include <gtest/gtest.h>
#include <vector>
#include <map>
#include <string>
#include "lib/lazy_adapters.h"

template<typename Range>
auto to_vec(Range&& range) {
    using T = std::decay_t<decltype(*std::begin(range))>;
    std::vector<T> result;
    for (auto&& item : range) result.push_back(item);
    return result;
}

TEST(FilterTest, FiltersOddNumbers) {
    std::vector<int> input{1,2,3,4,5,6};
    auto output = to_vec(input | pick_if([](int x) { return x % 2 != 0; }));
    std::vector<int> expected{1,3,5};
    EXPECT_EQ(output, expected);
}

TEST(TransformTest, SquaresElements) {
    std::vector<int> input{1,2,3};
    auto output = to_vec(input | apply([](int x) { return x * x; }));
    std::vector<int> expected{1,4,9};
    EXPECT_EQ(output, expected);
}

TEST(TakeTest, TakesFirstN) {
    std::vector<int> input{10,20,30,40,50};
    auto output = to_vec(input | first(3));
    std::vector<int> expected{10,20,30};
    EXPECT_EQ(output, expected);
}

TEST(DropTest, DropsFirstN) {
    std::vector<int> input{10,20,30,40,50};
    auto output = to_vec(input | skip(2));
    std::vector<int> expected{30,40,50};
    EXPECT_EQ(output, expected);
}

TEST(ReverseTest, ReversesSequence) {
    std::vector<int> input{1,2,3,4};
    auto output = to_vec(input | backwards());
    std::vector<int> expected{4,3,2,1};
    EXPECT_EQ(output, expected);
}

TEST(KeysTest, ExtractsMapKeys) {
    std::map<int, std::string> map{{1, "a"}, {2, "b"}, {3, "c"}};
    auto output = to_vec(map | get_keys());
    std::vector<int> expected{1,2,3};
    EXPECT_EQ(output, expected);
}

TEST(ValuesTest, ExtractsMapValues) {
    std::map<int, std::string> map{{1, "a"}, {2, "b"}, {3, "c"}};
    auto output = to_vec(map | get_values());
    std::vector<std::string> expected{"a","b","c"};
    EXPECT_EQ(output, expected);
}

TEST(PipelineTest, FilterThenTransform) {
    std::vector<int> input{1,2,3,4,5};
    auto pipe = input | pick_if([](int x){ return x % 2; }) | apply([](int x){ return x + 1; });
    auto result = to_vec(pipe);
    std::vector<int> expected{2,4,6};
    EXPECT_EQ(result, expected);
}
