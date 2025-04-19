#pragma once
#include <vector>
#include <string>
#include <cstdint>

class BmpWriter {
public:
    static void writeBMP(const std::string& filename, const std::vector<std::vector<uint64_t>>& field);
};
