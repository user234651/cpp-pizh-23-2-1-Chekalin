#include "../lib/parser.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace omfl;

TEST(ParserTestSuite, EmptyTest) {
    std::string data = "";
    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
}

TEST(ParserTestSuite, IntTest) {
    std::string data = R"(
        key1 = 100500
        key2 = -22
        key3 = +28)";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_TRUE(root.Get("key1").IsInt());
    EXPECT_TRUE(root.Get("key2").IsInt());
    EXPECT_TRUE(root.Get("key3").IsInt());
    EXPECT_EQ(root.Get("key1").AsInt(), 100500);
    EXPECT_EQ(root.Get("key2").AsInt(), -22);
    EXPECT_EQ(root.Get("key3").AsInt(), 28);
}

TEST(ParserTestSuite, InvalidIntTest) {
    std::string data = R"(
        key1 = true
        key2 = -22.1
        key3 = "ITMO")";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_FALSE(root.Get("key1").IsInt());
    EXPECT_FALSE(root.Get("key2").IsInt());
    EXPECT_FALSE(root.Get("key3").IsInt());
    EXPECT_THROW(root.Get("key1").AsInt(), std::runtime_error);
    EXPECT_THROW(root.Get("key2").AsInt(), std::runtime_error);
    EXPECT_THROW(root.Get("key3").AsInt(), std::runtime_error);
}

TEST(ParserTestSuite, FloatTest) {
    std::string data = R"(
        key1 = 2.1
        key2 = -3.14
        key3 = -0.001)";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_TRUE(root.Get("key1").IsFloat());
    EXPECT_TRUE(root.Get("key2").IsFloat());
    EXPECT_TRUE(root.Get("key3").IsFloat());
    EXPECT_FLOAT_EQ(root.Get("key1").AsFloat(), 2.1f);
    EXPECT_FLOAT_EQ(root.Get("key2").AsFloat(), -3.14f);
    EXPECT_FLOAT_EQ(root.Get("key3").AsFloat(), -0.001f);
}

TEST(ParserTestSuite, InvalidFloatTest) {
    std::string data = R"(
        key1 = true
        key2 = -2
        key3 = "ITMO")";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_FALSE(root.Get("key1").IsFloat());
    EXPECT_FALSE(root.Get("key2").IsFloat());
    EXPECT_FALSE(root.Get("key3").IsFloat());
    EXPECT_THROW(root.Get("key1").AsFloat(), std::runtime_error);
    EXPECT_THROW(root.Get("key2").AsFloat(), std::runtime_error);
    EXPECT_THROW(root.Get("key3").AsFloat(), std::runtime_error);
}

TEST(ParserTestSuite, StringTest) {
    std::string data = R"(
        key = "value"
        key1 = "value1")";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_TRUE(root.Get("key").IsString());
    EXPECT_TRUE(root.Get("key1").IsString());
    EXPECT_EQ(root.Get("key").AsString(), "value");
    EXPECT_EQ(root.Get("key1").AsString(), "value1");
}

TEST(ParserTestSuite, InvalidStringTest) {
    std::string data = R"(
        key = true
        key1 = ["1", "2", "3"])";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_FALSE(root.Get("key").IsString());
    EXPECT_FALSE(root.Get("key1").IsString());
    EXPECT_THROW(root.Get("key").AsString(), std::runtime_error);
    EXPECT_THROW(root.Get("key1").AsString(), std::runtime_error);
}

TEST(ParserTestSuite, ArrayTest) {
    std::string data = R"(
        key1 = [1, 2, 3, 4, 5, 6])";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_TRUE(root.Get("key1").IsArray());
    const auto& arr = root.Get("key1").AsArray();
    EXPECT_EQ(arr.at(0).AsInt(), 1);
    EXPECT_EQ(arr.at(1).AsInt(), 2);
    EXPECT_EQ(arr.at(2).AsInt(), 3);
    EXPECT_THROW(arr.at(100500).AsInt(), std::out_of_range);
}

TEST(ParserTestSuite, DiffTypesArrayTest) {
    std::string data = R"(
        key1 = [1, true, 3.14, "ITMO", [1, 2, 3], ["a", "b", 28]])";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    const auto& arr = root.Get("key1").AsArray();
    EXPECT_TRUE(arr.at(0).IsInt()); EXPECT_EQ(arr.at(0).AsInt(), 1);
    EXPECT_TRUE(arr.at(1).IsBool()); EXPECT_EQ(arr.at(1).AsBool(), true);
    EXPECT_TRUE(arr.at(2).IsFloat()); EXPECT_FLOAT_EQ(arr.at(2).AsFloat(), 3.14f);
    EXPECT_TRUE(arr.at(3).IsString()); EXPECT_EQ(arr.at(3).AsString(), "ITMO");
    const auto& sub1 = arr.at(4).AsArray();
    EXPECT_EQ(sub1.at(0).AsInt(), 1); EXPECT_EQ(sub1.at(1).AsInt(), 2); EXPECT_EQ(sub1.at(2).AsInt(), 3);
    const auto& sub2 = arr.at(5).AsArray();
    EXPECT_EQ(sub2.at(0).AsString(), "a"); EXPECT_EQ(sub2.at(1).AsString(), "b"); EXPECT_EQ(sub2.at(2).AsInt(), 28);
}

TEST(ParserTestSuite, CommentsTest) {
    std::string data = R"(
        key1 = 100500  # some important value

        # It's more than university)";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    EXPECT_EQ(root.Get("key1").AsInt(), 100500);
}

TEST(ParserTestSuite, SectionTest) {
    std::string data = R"(
        [section1]
        key1 = 1
        key2 = true

        [section1]
        key3 = "value")";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    const auto& sec = root.root_.Get("section1").AsSection();
    EXPECT_EQ(sec.Get("key1").AsInt(), 1);
    EXPECT_EQ(sec.Get("key2").AsBool(), true);
    EXPECT_EQ(sec.Get("key3").AsString(), "value");
}

TEST(ParserTestSuite, MultiSectionTest) {
    std::string data = R"(
        [level1]
        key1 = 1
        [level1.level2-1]
        key2 = 2

        [level1.level2-2]
        key3 = 3)";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    const auto& lvl1 = root.root_.Get("level1").AsSection();
    EXPECT_EQ(lvl1.Get("key1").AsInt(), 1);
    const auto& l2a = lvl1.Get("level2-1").AsSection(); EXPECT_EQ(l2a.Get("key2").AsInt(), 2);
    const auto& l2b = lvl1.Get("level2-2").AsSection(); EXPECT_EQ(l2b.Get("key3").AsInt(), 3);
}

TEST(ParserTestSuite, GetSectionTest) {
    std::string data = R"(
        [level1.level2.level3]
        key1 = 1)";

    Config root = Parse(data);
    ASSERT_TRUE(root.IsValid());
    const auto& lvl3 = root.root_.Get("level1").AsSection()
                                .Get("level2").AsSection()
                                .Get("level3").AsSection();
    EXPECT_EQ(lvl3.Get("key1").AsInt(), 1);
}
