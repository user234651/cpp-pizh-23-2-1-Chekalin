#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <memory>

// Разбор и отображение фреймов ID3v2
namespace id3 {

// Декодирует синхро-байты в целое (sync-safe integer)
uint32_t parseSyncSafe(const std::array<unsigned char,4>& bytes);

// Предварительное объявление базового класса
class ID3Frame;

// Класс текстового фрейма
class TextID3Frame;

// Класс URL-фрейма
class UrlID3Frame;

// Класс комментариев
class CommentID3Frame;

// Класс неизвестного фрейма
class UnknownID3Frame;

// Фабрика для создания соответствующего объекта по ID
std::unique_ptr<ID3Frame> makeFrame(const std::string& id,
                                    uint32_t length,
                                    uint16_t flags,
                                    const std::vector<unsigned char>& data);

// Базовый абстрактный класс для фреймов
class ID3Frame {
public:
    virtual ~ID3Frame() = default;
    // Отображает содержимое фрейма
    virtual void display() const = 0;

protected:
    ID3Frame(const std::string& id, uint32_t length,
             uint16_t flags, const std::vector<unsigned char>& data)
        : id_(id), length_(length), flags_(flags), bytes_(data)
    {}

    std::string             id_;
    uint32_t                length_;
    uint16_t                flags_;
    std::vector<unsigned char> bytes_;
};

// Описание фрейма по его идентификатору
std::string describeFrame(const std::string& id);

} // namespace id3
