#include <gtest/gtest.h>
#include <sstream>
#include <array>
#include <memory>
#include "MP3Parser.h"

using namespace id3;

// Тест: синхро-безопасный целочисленный парсер из нулей
TEST(SyncSafeParse, AllZeros) {
    std::array<unsigned char,4> b = {0,0,0,0};
    EXPECT_EQ(parseSyncSafe(b), 0u);
}

// Тест: синхро-безопасный парсер с образцом
TEST(SyncSafeParse, SampleBytes) {
    std::array<unsigned char,4> b = {0x00,0x00,0x02,0x10};
    // ((0<<7)|0)=0; ((0<<7)|0)=0; ((0<<7)|2)=2; ((2<<7)|0x10)=272
    EXPECT_EQ(parseSyncSafe(b), 272u);
}

// Тест: описание по известному ID
TEST(FrameDescribe, Known) {
    EXPECT_EQ(describeFrame("TIT2"), "Название");
}

// Тест: описание по неизвестному ID
TEST(FrameDescribe, Unknown) {
    EXPECT_EQ(describeFrame("XXXX"), "");
}

// Тест: вывод текстового фрейма
TEST(TextFrameDisplay, Basic) {
    std::vector<unsigned char> d = {0x00,'H','i'};
    auto f = makeFrame("TIT2", 3, 0, d);
    testing::internal::CaptureStdout();
    f->display();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "TIT2 (Название): Hi");
}

// Тест: вывод URL-фрейма
TEST(UrlFrameDisplay, Basic) {
    std::vector<unsigned char> d = {'h','t','t','p',':','/','/'};
    auto f = makeFrame("WXXX", 7, 0, d);
    testing::internal::CaptureStdout();
    f->display();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "WXXX (URL): http://");
}

// Тест: вывод комментария
TEST(CommentFrameDisplay, Basic) {
    // [0]=кодировка, [1..3]=язык, затем null и текст
    std::vector<unsigned char> d = {0x00,'e','n','g',0x00,'C','o','m','m','e','n','t'};
    auto f = makeFrame("COMM", d.size(), 0, d);
    testing::internal::CaptureStdout();
    f->display();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "COMM (Комментарий) [eng]: Comment");
}

// Тест: вывод неизвестного фрейма
TEST(UnknownFrameDisplay, Basic) {
    std::vector<unsigned char> d = {0x01,0x02};
    auto f = makeFrame("ABCD", 2, 0, d);
    testing::internal::CaptureStdout();
    f->display();
    std::string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out, "ABCD (2 bytes)");
}

// Тест: фабрика создаёт TextID3Frame при 'T'
TEST(FrameFactory, TextCreation) {
    std::vector<unsigned char> d = {0x00};
    auto ptr = makeFrame("TABC",1,0,d);
    EXPECT_NE(dynamic_cast<TextID3Frame*>(ptr.get()), nullptr);
}

// Тест: фабрика создаёт UrlID3Frame при 'W'
TEST(FrameFactory, UrlCreation) {
    std::vector<unsigned char> d = {'x'};
    auto ptr = makeFrame("WABC",1,0,d);
    EXPECT_NE(dynamic_cast<UrlID3Frame*>(ptr.get()), nullptr);
}
