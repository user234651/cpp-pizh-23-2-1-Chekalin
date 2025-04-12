#include "word_count.h"
#include <fstream>
#include <cctype>

int countLines(const std::string& filename) {
    std::ifstream file(filename);
    int lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++lines;
    }
    return lines;
}

int countWords(const std::string& filename) {
    std::ifstream file(filename);
    int words = 0;
    std::string word;
    while (file >> word) {
        ++words;
    }
    return words;
}

int countBytes(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    return file.tellg();
}

int countChars(const std::string& filename) {
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
