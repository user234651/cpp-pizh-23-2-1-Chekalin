// ArgParser.h
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace ArgumentParser {

class ArgParser {
public:
    ArgParser(const std::string& programName);

    // Parse from argc/argv or a vector of strings
    bool Parse(int argc, char** argv);
    bool Parse(const std::vector<std::string>& args);

    // Check if help was requested
    bool Help() const;

    // Generate help description
    std::string HelpDescription() const;

    // Retrieve parsed values
    std::string GetStringValue(const std::string& name) const;
    int GetIntValue(const std::string& name) const;
    int GetIntValue(const std::string& name, size_t index) const;
    bool GetFlag(const std::string& name) const;

    // Builder for defining arguments
    class ArgBuilder;

    // Add different kinds of arguments
    ArgBuilder AddStringArgument(const std::string& name, const std::string& description = "");
    ArgBuilder AddStringArgument(char shortName, const std::string& name, const std::string& description = "");

    ArgBuilder AddIntArgument(const std::string& name, const std::string& description = "");
    ArgBuilder AddIntArgument(char shortName, const std::string& name, const std::string& description = "");

    ArgBuilder AddFlag(const std::string& name, const std::string& description = "");
    ArgBuilder AddFlag(char shortName, const std::string& name, const std::string& description = "");

    ArgBuilder AddHelp(char shortName, const std::string& name, const std::string& description);

private:
    std::string programName_;
    bool helpRequested_ = false;

    enum class ArgType { String, Int, Flag, Help };
    struct Option {
        ArgType type;
        std::string longName;
        char shortName;
        std::string description;
        bool positional = false;
        bool multi = false;
        size_t minCount = 0;
        bool seen = false;
        bool hasDefault = false;
        std::string defaultString;
        int defaultInt = 0;
        bool defaultBool = false;
        std::string valueString;
        std::vector<std::string> valuesString;
        int valueInt = 0;
        std::vector<int> valuesInt;
        bool valueBool = false;
        std::string* storeString = nullptr;
        std::vector<std::string>* storeStrings = nullptr;
        int* storeInt = nullptr;
        std::vector<int>* storeInts = nullptr;
        bool* storeBool = nullptr;
    };

    std::vector<Option> options_;
    std::unordered_map<std::string, size_t> longNameMap_;
    std::unordered_map<char, size_t> shortNameMap_;

    Option& CreateOption(ArgType type, char shortName, const std::string& longName, const std::string& description);

public:
    class ArgBuilder {
    public:
        ArgBuilder(ArgParser& parser, size_t index);

        ArgBuilder& Default(const std::string& defaultValue);
        ArgBuilder& Default(int defaultValue);
        ArgBuilder& Default(bool defaultValue);

        ArgBuilder& StoreValue(std::string& out);
        ArgBuilder& StoreValues(std::vector<std::string>& out);
        ArgBuilder& StoreValue(int& out);
        ArgBuilder& StoreValues(std::vector<int>& out);
        ArgBuilder& StoreValue(bool& out);

        ArgBuilder& MultiValue(size_t minCount = 0);
        ArgBuilder& Positional();

    private:
        ArgParser& parser_;
        size_t index_;
    };
};

} // namespace ArgumentParser