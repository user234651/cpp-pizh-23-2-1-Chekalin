#include "MP3Parser.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cstring>

namespace id3 {

// ----------- parseSyncSafe implementation ------------
uint32_t parseSyncSafe(const std::array<unsigned char,4>& bytes) {
    uint32_t result = 0;
    for (int i = 0; i < 4; ++i) {
        result = (result << 7) | (bytes[i] & 0x7F);
    }
    return result;
}

// ----------- describeFrame implementation ------------
std::string describeFrame(const std::string& id) {
    static const std::unordered_map<std::string,std::string> mapDescr = {
        {"TIT2","Название"},   {"TPE1","Исполнитель"},
        {"TALB","Альбом"},     {"TRCK","Номер трека"},
        {"TYER","Год"},        {"TDRC","Дата релиза"},
        {"TCON","Жанр"},       {"COMM","Комментарий"},
        {"TENC","Кодировщик"}, {"TCOM","Композитор"},
        {"TBPM","Темп"},       {"TSSE","Программа"},
        {"TPOS","Номер диска"}
    };
    auto it = mapDescr.find(id);
    return it != mapDescr.end() ? it->second : "";
}

// ----------- ID3Frame ctor (определение) ------------
ID3Frame::ID3Frame(const std::string& id, uint32_t length,
                   uint16_t flags, const std::vector<unsigned char>& data)
    : id_(id), length_(length), flags_(flags), bytes_(data) {}

// ----------- TextID3Frame implementation ------------
class TextID3Frame : public ID3Frame {
public:
    using ID3Frame::ID3Frame;
    void display() const override {
        std::string txt;
        if (!bytes_.empty()) {
            // первый байт — кодировка
            txt.assign(bytes_.begin() + 1, bytes_.end());
        }
        std::string desc = describeFrame(id_);
        std::cout << id_;
        if (!desc.empty()) std::cout << " (" << desc << ")";
        std::cout << ": " << txt;
    }
};

// ----------- UrlID3Frame implementation ------------
class UrlID3Frame : public ID3Frame {
public:
    using ID3Frame::ID3Frame;
    void display() const override {
        std::string url(bytes_.begin(), bytes_.end());
        std::string desc = describeFrame(id_);
        std::cout << id_;
        if (!desc.empty()) std::cout << " (" << desc << ")";
        std::cout << " (URL): " << url;
    }
};

// ----------- CommentID3Frame implementation ------------
class CommentID3Frame : public ID3Frame {
public:
    using ID3Frame::ID3Frame;
    void display() const override {
        if (bytes_.size() < 4) {
            std::cout << id_ << ": <некорректный COMM>";
            return;
        }
        // bytes_[0] — кодировка, [1..3] — язык
        std::string lang(reinterpret_cast<const char*>(&bytes_[1]), 3);
        size_t pos = 4;
        // пропускаем пустой дескриптор
        while (pos < bytes_.size() && bytes_[pos] != 0) ++pos;
        ++pos;
        std::string txt(bytes_.begin() + pos, bytes_.end());
        std::string desc = describeFrame(id_);
        std::cout << id_;
        if (!desc.empty()) std::cout << " (" << desc << ")";
        std::cout << " [" << lang << "]: " << txt;
    }
};

// ----------- UnknownID3Frame implementation ------------
class UnknownID3Frame : public ID3Frame {
public:
    using ID3Frame::ID3Frame;
    void display() const override {
        std::cout << id_ << " (" << length_ << " bytes)";
    }
};

// ----------- makeFrame factory ------------
std::unique_ptr<ID3Frame> makeFrame(const std::string& id,
                                    uint32_t length,
                                    uint16_t flags,
                                    const std::vector<unsigned char>& data) {
    if (!id.empty() && id[0] == 'T') {
        return std::make_unique<TextID3Frame>(id, length, flags, data);
    }
    if (!id.empty() && id[0] == 'W') {
        return std::make_unique<UrlID3Frame>(id, length, flags, data);
    }
    if (id == "COMM") {
        return std::make_unique<CommentID3Frame>(id, length, flags, data);
    }
    return std::make_unique<UnknownID3Frame>(id, length, flags, data);
}

} // namespace id3
