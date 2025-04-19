#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Sandpile {
public:
    Sandpile(int width, int height);
    void loadFromFile(const std::string& path);
    void run(uint64_t maxIterations = 100000, uint64_t freq = 0, const std::string& inputFilename = "");
    const std::vector<std::vector<uint64_t>>& getField() const;

private:
    void topple();
    bool isStable() const;
    void saveBMP(const std::string& inputFilename) const;

    int width;
    int height;
    std::vector<std::vector<uint64_t>> field;
};
