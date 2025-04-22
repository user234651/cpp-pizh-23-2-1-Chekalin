#include <iostream>
#include <fstream>
#include <sstream>
#include "lib/parser.h"

using namespace omfl; // подключаем пространство имён для удобства

int main(int argc, char* argv[]) {
    // Determine config file path
    std::string path = "../example/config.omfl";
    if (argc > 1) {
        path = argv[1];
    }

    // Read file content
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open config file: " << path << std::endl;
        return 1;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();

    // Parse OMFL config
    Config cfg = Parse(content);
    if (!cfg.IsValid()) {
        std::cerr << "Config parsing failed." << std::endl;
        return 2;
    }

    // Example: retrieve values
    try {
        int version = cfg.Get("version").AsInt();
        std::string name = cfg.Get("name").AsString();
        bool enabled = cfg.Get("features").AsSection().Get("enabled").AsBool();

        std::string comment = cfg.Get("university").AsSection().Get("pizh").AsSection().Get("student").AsString();
        std::cout << "university >> pizh >> student: " << comment << std::endl;

        std::cout << "Version: " << version << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Features.enable: " << (enabled ? "true" : "false") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error accessing config values: " << e.what() << std::endl;
        return 3;
    }

    return 0;
}