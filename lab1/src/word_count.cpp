#include "word_count.h"
#include <fstream>
#include <cctype>

WordCounter::WordCounter(const std::string& filename) : filename(filename) {}

int WordCounter::countLines() const {
    std::ifstream file(filename);
    int lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++lines;
    }
    return lines;
}

int WordCounter::countWords() const {
    std::ifstream file(filename);
    int words = 0;
    std::string word;
    while (file >> word) {
        ++words;
    }
    return words;
}

int WordCounter::countBytes() const {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.tellg();
}

int WordCounter::countChars() const {
    std::ifstream file(filename);
    int chars = 0;
    char c;
    while (file.get(c)) {
        if (std::isalpha(c)) {
            ++chars;
        }
    }
    return chars;
}
