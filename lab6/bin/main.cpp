#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <array>
#include <locale>
#include "lib/MP3Parser.h"

using namespace id3;

int main(int argc, char* argv[]) {
    // Устанавливаем локаль для корректного вывода
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <mp3-файл>\n";
        return 1;
    }
    const char* path = argv[1];
    std::cout << "Обрабатываем файл: " << path << std::endl;

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cerr << "Ошибка открытия файла: " << path << "\n";
        return 1;
    }

    // Считываем заголовок ID3v2
    char tagHeader[10];
    in.read(tagHeader, 10);
    if (in.gcount() != 10 || std::strncmp(tagHeader, "ID3", 3) != 0) {
        std::cerr << "Тег ID3v2 не найден." << std::endl;
        return 1;
    }

    unsigned char majorVer  = static_cast<unsigned char>(tagHeader[3]);
    unsigned char minorVer  = static_cast<unsigned char>(tagHeader[4]);
    unsigned char hdrFlags  = static_cast<unsigned char>(tagHeader[5]);
    std::array<unsigned char,4> syncBytes = {
        static_cast<unsigned char>(tagHeader[6]),
        static_cast<unsigned char>(tagHeader[7]),
        static_cast<unsigned char>(tagHeader[8]),
        static_cast<unsigned char>(tagHeader[9])
    };
    uint32_t tagLength = parseSyncSafe(syncBytes);
    std::cout << "ID3v2." << int(majorVer) << "." << int(minorVer)
              << " тег, размер=" << tagLength << "\n";

    uint32_t processed = 0;
    while (processed + 10 <= tagLength) {
        char frameIdBuf[4];
        in.read(frameIdBuf, 4);
        if (in.gcount() != 4 ||
            std::all_of(frameIdBuf, frameIdBuf + 4, [](char c){ return c == 0; })) {
            break;
        }
        std::string fid(frameIdBuf, 4);

        std::array<unsigned char,4> frameSync;
        in.read(reinterpret_cast<char*>(frameSync.data()), 4);
        uint32_t payloadLen = parseSyncSafe(frameSync);

        uint16_t hdrFlagsFrame;
        in.read(reinterpret_cast<char*>(&hdrFlagsFrame), 2);

        if (payloadLen == 0 || in.eof()) {
            break;
        }

        std::vector<unsigned char> payload(payloadLen);
        in.read(reinterpret_cast<char*>(payload.data()), payloadLen);
        if (in.gcount() != static_cast<std::streamsize>(payloadLen)) {
            break;
        }

        processed += 10 + payloadLen;

        auto framePtr = makeFrame(fid, payloadLen, hdrFlagsFrame, payload);
        framePtr->display();
        std::cout << std::endl;
    }

    return 0;
}
