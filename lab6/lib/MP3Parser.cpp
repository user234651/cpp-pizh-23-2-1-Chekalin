#include "MP3Parser.h"
#include <map>
#include <cstring>
#include <algorithm>

uint32_t readSynchSafeInteger(const std::array<unsigned char,4>& bytes) {
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value = (value << 7) | (bytes[i] & 0x7F);
    }
    return value;
}

Frame::Frame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_)
    : id(id_), size(size_), flags(flags_), data(data_) {}

void Frame::print() const {
    std::cout << id << " (" << size << " bytes)";
}

std::string getFrameDescription(const std::string& id) {
    static const std::unordered_map<std::string, std::string> descriptions = {
        {"TIT2", "Название"}, {"TPE1", "Исполнитель"}, {"TALB", "Альбом"},
        {"TRCK", "Номер трека"}, {"TYER", "Год"}, {"TDRC", "Дата релиза"},
        {"TCON", "Жанр"}, {"COMM", "Комментарий"}, {"TENC", "Программа кодирования"},
        {"TCOM", "Композитор"}, {"TBPM", "Темп (BPM)"}, {"TSSE", "Программа кодирования"},
        {"TPOS", "Номер диска"}
    };
    auto it = descriptions.find(id);
    return it != descriptions.end() ? it->second : "";
}

TextFrame::TextFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_)
    : Frame(id_, size_, flags_, data_) {}

void TextFrame::print() const {
    std::string text;
    if (!data.empty()) {
        unsigned char enc = data[0];
        text.assign(data.begin() + 1, data.end());
    }
    std::string desc = getFrameDescription(id);
    std::cout << id;
    if (!desc.empty()) std::cout << " (" << desc << ")";
    std::cout << ": " << text;
}

UrlFrame::UrlFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_)
    : Frame(id_, size_, flags_, data_) {}

void UrlFrame::print() const {
    std::string url(data.begin(), data.end());
    std::string desc = getFrameDescription(id);
    std::cout << id;
    if (!desc.empty()) std::cout << " (" << desc << ")";
    std::cout << " (URL): " << url;
}

CommentFrame::CommentFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_)
    : Frame(id_, size_, flags_, data_) {}

void CommentFrame::print() const {
    if (data.size() < 4) {
        std::cout << id << ": <invalid COMM frame>";
        return;
    }
    unsigned char enc = data[0];
    std::string language(reinterpret_cast<const char*>(&data[1]), 3);
    size_t pos = 4;
    while (pos < data.size() && data[pos] != 0) ++pos;
    ++pos;
    std::string text(data.begin() + pos, data.end());
    std::string desc = getFrameDescription(id);
    std::cout << id;
    if (!desc.empty()) std::cout << " (" << desc << ")";
    std::cout << " [" << language << "]: " << text;
}

UnknownFrame::UnknownFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_)
    : Frame(id_, size_, flags_, data_) {}

std::unique_ptr<Frame> createFrame(const std::string& id, uint32_t size, uint16_t flags, const std::vector<unsigned char>& data) {
    if (!id.empty() && id[0] == 'T') {
        return std::make_unique<TextFrame>(id, size, flags, data);
    } else if (!id.empty() && id[0] == 'W') {
        return std::make_unique<UrlFrame>(id, size, flags, data);
    } else if (id == "COMM") {
        return std::make_unique<CommentFrame>(id, size, flags, data);
    } else {
        return std::make_unique<UnknownFrame>(id, size, flags, data);
    }
}