#include "./lib/parser.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace omfl;

class ValidFormatTestSuite : public testing::TestWithParam<const char*> {
};

TEST_P(ValidFormatTestSuite, ValidTest) {
    std::string param = GetParam();
    Config cfg = Parse(param);
    ASSERT_TRUE(cfg.IsValid());
}

class InvalidFormatTestSuite : public testing::TestWithParam<const char*> {
};

TEST_P(InvalidFormatTestSuite, InValidTest) {
    std::string param = GetParam();
    Config cfg = Parse(param);
    ASSERT_FALSE(cfg.IsValid());
}

// Key
INSTANTIATE_TEST_SUITE_P(
    KeyValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key = \"value\"",
        " key      =  \"value\"   ",
        " key=  \"value\"",
        " key123 =  \"value\"",
        " key_1-23-abcd =  \"value\"",
        " 23key_ =  \"value\""
    )
);

INSTANTIATE_TEST_SUITE_P(
    KeyValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key = \n\"value\"",
        "      =  \"value\"   ",
        "=  \"value\""
    )
);

// Value
INSTANTIATE_TEST_SUITE_P(
    ValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key = \"value\""
    )
);

INSTANTIATE_TEST_SUITE_P(
    ValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key = ",
        "key = abcd"
    )
);

TEST(FormatTestSuite, KeyRedefinitionTest) {
    std::string data = R"(
        key = 1
        key = 2)";
    Config cfg = Parse(data);
    ASSERT_FALSE(cfg.IsValid());
}

// Integer Value
INSTANTIATE_TEST_SUITE_P(
    IntValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key1 = 2",
        "key2 = -22",
        "key3 = +48",
        "key4 = 2-2",
        "key5 = 4+8"
    )
);

INSTANTIATE_TEST_SUITE_P(
    IntValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key1 = -+",
        "key2 = +"
    )
);

// Float Value
INSTANTIATE_TEST_SUITE_P(
    FloatValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key1 = 3.14",
        "key2 = -3.14",
        "key3 = +0.00001"
    )
);

INSTANTIATE_TEST_SUITE_P(
    FloatValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key1 = +.",
        "key2 = ."
    )
);

// String Value
INSTANTIATE_TEST_SUITE_P(
    StringValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key1 = \"Hello world\"",
        "key2 = \"1, 2, 3, 4, 5\"",
        "key3 = \"3.14\"",
        "key4 = \"1\t2\t3\"",
        "key5 = \"[1,2,3,4,5]\""        
    )
);

INSTANTIATE_TEST_SUITE_P(
    StringValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key1 = \"Hello world",
        "key2 = \"Bjarne\" \"stroustrup"
    )
);

// Bool Value
INSTANTIATE_TEST_SUITE_P(
    BoolValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key1 = true",
        "key2 = false"
    )
);

INSTANTIATE_TEST_SUITE_P(
    BoolValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key1 = tru",
        "key2 = alse",
        "key3 = true true",
        "key4 = fal se",
        "key5 = truefalse"
    )
);

// Array Value
INSTANTIATE_TEST_SUITE_P(
    ArrayValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key1 = []",
        "key2 = [1,2,3,4,5]",
        "key3 = [1, -3.14, true, \"ITMO\"]",
        "key4 = [[1,2],[2,[3,4,5]]]"
    )
);

INSTANTIATE_TEST_SUITE_P(
    ArrayValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "key1 = [",
        "key2 = ]",
        "key3 = [1,2,3,4",
        "key4 = [[1,2],[2,[3,4,5]]"
    )
);

// Section
INSTANTIATE_TEST_SUITE_P(
    SectionValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "[section-1]",
        "[section-1.section-2]",
        "[a.b.c.d]"
    )
);

INSTANTIATE_TEST_SUITE_P(
    SectionValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "[}",
        "{section-1}",
        "[section-1.}",
        "[.section-1}"
    )
);

// Comment
INSTANTIATE_TEST_SUITE_P(
    CommentValueValidation,
    ValidFormatTestSuite,
    testing::Values(
        "key1 = 1 # comment",
        "# some text"
    )
);

INSTANTIATE_TEST_SUITE_P(
    CommentValueValidation,
    InvalidFormatTestSuite,
    testing::Values(
        "# comment \n newline comment"
    )
);

TEST(ExtraTests, WhitespaceOnly) {
    Config cfg = Parse("");
    ASSERT_TRUE(cfg.IsValid());
}

TEST(ExtraTests, SingleCommentLine) {
    Config cfg = Parse("# configuration header");
    ASSERT_TRUE(cfg.IsValid());
}

TEST(ExtraTests, ZeroIntValue) {
    Config cfg = Parse("key0 = 0");
    ASSERT_TRUE(cfg.IsValid());
    EXPECT_TRUE(cfg.Get("key0").IsInt());
    EXPECT_EQ(cfg.Get("key0").AsInt(), 0);
}

TEST(ExtraTests, PositiveFloatValue) {
    Config cfg = Parse("pi = 0.1");
    ASSERT_TRUE(cfg.IsValid());
    EXPECT_TRUE(cfg.Get("pi").IsFloat());
    EXPECT_FLOAT_EQ(cfg.Get("pi").AsFloat(), 0.1f);
}

TEST(ExtraTests, NestedEmptyArray) {
    Config cfg = Parse("arr = [[]]");
    ASSERT_TRUE(cfg.IsValid());
    const auto& outer = cfg.Get("arr").AsArray();
    ASSERT_EQ(outer.size(), 1u);
    EXPECT_TRUE(outer[0].IsArray());
    EXPECT_TRUE(outer[0].AsArray().empty());
}

TEST(ExtraTests, HashInsideString) {
    Config cfg = Parse("s = \"#notacomment\"");
    ASSERT_TRUE(cfg.IsValid());
    EXPECT_EQ(cfg.Get("s").AsString(), "#notacomment");
}

TEST(ExtraTests, EmptyArrayTest) {
    Config cfg = Parse("a = []");
    ASSERT_TRUE(cfg.IsValid());
    EXPECT_TRUE(cfg.Get("a").IsArray());
    EXPECT_TRUE(cfg.Get("a").AsArray().empty());
}

TEST(ExtraTests, MultipleCommentLines) {
    Config cfg = Parse("# first comment # second comment # third comment");
    ASSERT_TRUE(cfg.IsValid());
}

TEST(ExtraTests, NumericStringValue) {
    Config cfg = Parse("num = \"12345\"");
    ASSERT_TRUE(cfg.IsValid());
    EXPECT_TRUE(cfg.Get("num").IsString());
    EXPECT_EQ(cfg.Get("num").AsString(), "12345");
}

TEST(ExtraTests, SectionThenKeyTest) {
    Config cfg = Parse("[sec] sub = 5");
    ASSERT_TRUE(cfg.IsValid());
}