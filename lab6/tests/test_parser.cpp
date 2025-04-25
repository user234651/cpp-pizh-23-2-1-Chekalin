#include <gtest/gtest.h>
#include <sstream>
#include "lib/MP3Parser.h"

// Тест: Синхросейф-интегер из нулей должен дать 0
TEST(SynchSafeInteger, AllZeroBytes) {
    std::array<unsigned char,4> bytes = {0x00, 0x00, 0x00, 0x00};
    EXPECT_EQ(readSynchSafeInteger(bytes), 0u);
}

// Тест: Синхросейф-интегер с конкретными значениями
TEST(SynchSafeInteger, MixedBytes) {
    std::array<unsigned char,4> bytes = {0x00, 0x00, 0x02, 0x10};
    // ((0<<7)|0) -> 0; ((0<<7)|0) -> 0; ((0<<7)|2) -> 2; ((2<<7)|0x10) -> 272
    EXPECT_EQ(readSynchSafeInteger(bytes), 272u);
}

// Тест: Описание фрейма по известному ID
TEST(FrameDescription, KnownId) {
    EXPECT_EQ(getFrameDescription("TIT2"), "Название");
}

// Тест: Описание фрейма по неизвестному ID
TEST(FrameDescription, UnknownId) {
    EXPECT_EQ(getFrameDescription("ZZZZ"), "");
}

// Тест: Вывод текстового фрейма (TextFrame)
TEST(TextFramePrint, Basic) {
    std::vector<unsigned char> data = {0x00, 'H','i'};
    TextFrame frame("TIT2", 3, 0, data);
    testing::internal::CaptureStdout();
    frame.print();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "TIT2 (Название): Hi");
}

// Тест: Вывод URL фрейма (UrlFrame)
TEST(UrlFramePrint, Basic) {
    std::vector<unsigned char> data = {'h','t','t','p',':','/','/'};
    UrlFrame frame("WXXX", 7, 0, data);
    testing::internal::CaptureStdout();
    frame.print();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "WXXX (URL): http://");
}

// Тест: Вывод комментария (CommentFrame)
TEST(CommentFramePrint, Basic) {
    // encoding=0, lang="eng", short desc (empty), text after null
    std::vector<unsigned char> data = {0x00, 'e','n','g', 0x00, 'C','o','m','m','e','n','t'};
    CommentFrame frame("COMM", data.size(), 0, data);
    testing::internal::CaptureStdout();
    frame.print();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "COMM (Комментарий) [eng]: Comment");
}

// Тест: Вывод неизвестного фрейма (UnknownFrame)
TEST(UnknownFramePrint, Basic) {
    std::vector<unsigned char> data = {0x01,0x02};
    UnknownFrame frame("XXXX", 2, 0, data);
    testing::internal::CaptureStdout();
    frame.print();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "XXXX (2 bytes)");
}

// Тест: Фабрика должна вернуть TextFrame при ID, начинающемся с 'T'
TEST(FrameFactory, TextFrameCreation) {
    std::vector<unsigned char> data = {0x00};
    auto ptr = createFrame("TABC", 1, 0, data);
    EXPECT_NE(dynamic_cast<TextFrame*>(ptr.get()), nullptr);
}

// Тест: Фабрика должна вернуть UrlFrame при ID, начинающемся с 'W'
TEST(FrameFactory, UrlFrameCreation) {
    std::vector<unsigned char> data = {'x'};
    auto ptr = createFrame("WABC", 1, 0, data);
    EXPECT_NE(dynamic_cast<UrlFrame*>(ptr.get()), nullptr);
}