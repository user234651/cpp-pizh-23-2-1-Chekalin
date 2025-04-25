/* File: lib/MP3Parser.h */
#ifndef MP3PARSER_H
#define MP3PARSER_H

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include <unordered_map>

// Utility to read synchsafe integer (4 bytes)
uint32_t readSynchSafeInteger(const std::array<unsigned char,4>& bytes);

// Base class for all ID3 frames
class Frame {
public:
    std::string id;
    uint32_t size;
    uint16_t flags;
    std::vector<unsigned char> data;

    Frame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_);
    virtual ~Frame() = default;
    virtual void print() const;
};

std::string getFrameDescription(const std::string& id);

// Text frame: IDs starting with 'T'
class TextFrame : public Frame {
public:
    TextFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_);
    void print() const override;
};

// URL frame: IDs starting with 'W'
class UrlFrame : public Frame {
public:
    UrlFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_);
    void print() const override;
};

// Comment frame: COMM
class CommentFrame : public Frame {
public:
    CommentFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_);
    void print() const override;
};

// Generic unknown frame
class UnknownFrame : public Frame {
public:
    UnknownFrame(const std::string& id_, uint32_t size_, uint16_t flags_, const std::vector<unsigned char>& data_);
};

// Factory to create appropriate Frame subclass
std::unique_ptr<Frame> createFrame(const std::string& id,
                                   uint32_t size,
                                   uint16_t flags,
                                   const std::vector<unsigned char>& data);

#endif // MP3PARSER_H