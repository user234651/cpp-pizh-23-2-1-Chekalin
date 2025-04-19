#include "Sandpile.h"
#include "BmpWriter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

Sandpile::Sandpile(int width, int height) : width(width), height(height) {
    field.resize(height, std::vector<uint64_t>(width, 0));
}

void Sandpile::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + path);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int x, y;
        uint64_t grains;
        if (!(iss >> x >> y >> grains)) continue;
        if (y >= height || x >= width) continue;
        field[y][x] += grains;
    }
}

void Sandpile::run(uint64_t maxIterations, uint64_t freq, const std::string& inputFilename) {
    for (uint64_t i = 0; i < maxIterations; ++i) {
        if (freq > 0 && i % freq == 0) {
            saveBMP(inputFilename + "_" + std::to_string(i));
        }
        if (isStable()) break;
        topple();
    }
    saveBMP(inputFilename); // Финальный снимок
}

bool Sandpile::isStable() const {
    for (const auto& row : field)
        for (uint64_t cell : row)
            if (cell > 3) return false;
    return true;
}

void Sandpile::topple() {
    std::vector<std::vector<uint64_t>> next = field;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (field[y][x] >= 4) {
                next[y][x] -= 4;
                if (y > 0) next[y - 1][x]++;
                if (y + 1 < height) next[y + 1][x]++;
                if (x > 0) next[y][x - 1]++;
                if (x + 1 < width) next[y][x + 1]++;
            }
        }
    }
    field = std::move(next);
}

void Sandpile::saveBMP(const std::string& inputFilename) const {
    std::string base = std::filesystem::path(inputFilename).stem().string(); // main_input
    std::string bmpPath = base + ".bmp";
    BmpWriter::writeBMP(bmpPath, field);
}

const std::vector<std::vector<uint64_t>>& Sandpile::getField() const {
    return field;
}
