#ifndef WORD_COUNT_H
#define WORD_COUNT_H

#include <string>

class WordCounter {
private:
    std::string filename;

public:
    WordCounter(const std::string& filename);

    int countLines() const;
    int countWords() const;
    int countBytes() const;
    int countChars() const;
};

#endif // WORD_COUNT_H
