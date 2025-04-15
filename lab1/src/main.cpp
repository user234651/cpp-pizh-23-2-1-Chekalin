#include <iostream>
#include <vector>
#include <string>
#include "word_count.h"

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Usage example: WordCount.exe [OPTION] filename [filename,...]\n";
        return 1;
    }

    std::vector<std::string> filenames;

    bool linesOutput = false;
    bool wordsOutput = false;
    bool bytesOutput = false;
    bool charsOutput = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-l") {
            linesOutput = true;
        } else if (arg == "-w") {
            wordsOutput = true;
        } else if (arg == "-c") {
            bytesOutput = true;
        } else if (arg == "-m") {
            charsOutput = true;
        } else {
            filenames.push_back(arg);
        }
    }

    for (const auto& filename : filenames) {
        WordCounter counter(filename);
        std::cout << "File " << filename << ": " << std::endl;

        if (linesOutput) {
            std::cout << "number of lines = " << counter.countLines() << std::endl;
        }
        if (wordsOutput) {
            std::cout << "number of words = " << counter.countWords() << std::endl;
        }
        if (bytesOutput) {
            std::cout << "number of bytes = " << counter.countBytes() << std::endl;
        }
        if (charsOutput) {
            std::cout << "number of chars = " << counter.countChars() << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}
