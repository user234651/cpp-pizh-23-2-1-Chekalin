#include "ArgParser.h"
#include <sstream>

namespace ArgumentParser {

ArgParser::ArgParser(const std::string& programName)
    : programName_(programName) {}

ArgParser::Option& ArgParser::CreateOption(ArgType type, char shortName, const std::string& longName, const std::string& description) {
    options_.push_back({});
    Option& opt = options_.back();
    opt.type = type;
    opt.longName = longName;
    opt.shortName = shortName;
    opt.description = description;
    if (type == ArgType::Flag || type == ArgType::Help) {
        opt.valueBool = false;
    }
    size_t idx = options_.size() - 1;
    if (!longName.empty()) longNameMap_[longName] = idx;
    if (shortName) shortNameMap_[shortName] = idx;
    return opt;
}

// ArgBuilder implementations
ArgParser::ArgBuilder::ArgBuilder(ArgParser& parser, size_t index)
    : parser_(parser), index_(index) {}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::Default(const std::string& defaultValue) {
    auto& opt = parser_.options_[index_];
    opt.hasDefault = true;
    opt.defaultString = defaultValue;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::Default(int defaultValue) {
    auto& opt = parser_.options_[index_];
    opt.hasDefault = true;
    opt.defaultInt = defaultValue;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::Default(bool defaultValue) {
    auto& opt = parser_.options_[index_];
    opt.hasDefault = true;
    opt.defaultBool = defaultValue;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::StoreValue(std::string& out) {
    parser_.options_[index_].storeString = &out;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::StoreValues(std::vector<std::string>& out) {
    parser_.options_[index_].storeStrings = &out;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::StoreValue(int& out) {
    parser_.options_[index_].storeInt = &out;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::StoreValues(std::vector<int>& out) {
    parser_.options_[index_].storeInts = &out;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::StoreValue(bool& out) {
    parser_.options_[index_].storeBool = &out;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::MultiValue(size_t minCount) {
    auto& opt = parser_.options_[index_];
    opt.multi = true;
    opt.minCount = minCount;
    return *this;
}

ArgParser::ArgBuilder& ArgParser::ArgBuilder::Positional() {
    parser_.options_[index_].positional = true;
    return *this;
}

// Add argument methods
ArgParser::ArgBuilder ArgParser::AddStringArgument(const std::string& name, const std::string& description) {
    CreateOption(ArgType::String, 0, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}
ArgParser::ArgBuilder ArgParser::AddStringArgument(char shortName, const std::string& name, const std::string& description) {
    CreateOption(ArgType::String, shortName, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}
ArgParser::ArgBuilder ArgParser::AddIntArgument(const std::string& name, const std::string& description) {
    CreateOption(ArgType::Int, 0, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}
ArgParser::ArgBuilder ArgParser::AddIntArgument(char shortName, const std::string& name, const std::string& description) {
    CreateOption(ArgType::Int, shortName, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}
ArgParser::ArgBuilder ArgParser::AddFlag(const std::string& name, const std::string& description) {
    CreateOption(ArgType::Flag, 0, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}
ArgParser::ArgBuilder ArgParser::AddFlag(char shortName, const std::string& name, const std::string& description) {
    CreateOption(ArgType::Flag, shortName, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}
ArgParser::ArgBuilder ArgParser::AddHelp(char shortName, const std::string& name, const std::string& description) {
    CreateOption(ArgType::Help, shortName, name, description);
    return ArgBuilder(*this, options_.size() - 1);
}

// Parsing implementation
bool ArgParser::Parse(const std::vector<std::string>& args) {
    for (auto& opt : options_) {
        opt.seen = false;
        opt.valuesString.clear();
        opt.valuesInt.clear();
    }
    helpRequested_ = false;

    Option* positionalOpt = nullptr;
    for (auto& opt : options_) if (opt.positional) { positionalOpt = &opt; break; }

    for (size_t i = 1; i < args.size(); ++i) {
        const auto& arg = args[i];
        if (arg.rfind("--", 0) == 0) {
            if (arg == "--help") {
                helpRequested_ = true;
                continue;
            }
            auto posEq = arg.find('=');
            std::string name = arg.substr(2, posEq - 2);
            if (!longNameMap_.count(name)) return false;
            Option* opt = &options_[longNameMap_[name]];
            if (opt->type == ArgType::Flag) { opt->valueBool = true; opt->seen = true; }
            else {
                if (posEq == std::string::npos) return false;
                std::string val = arg.substr(posEq + 1);
                if (opt->type == ArgType::String) { if (opt->multi) opt->valuesString.push_back(val); else opt->valueString = val; }
                else if (opt->type == ArgType::Int) { try { int v = std::stoi(val); if (opt->multi) opt->valuesInt.push_back(v); else opt->valueInt = v; } catch(...) { return false; } }
                opt->seen = true;
            }
        } else if (arg.rfind("-", 0) == 0) {
            if (arg.size() == 2 && arg[1] == 'h') { helpRequested_ = true; continue; }
            if (arg.size() > 2 && arg[2] == '=') {
                char sn = arg[1];
                if (!shortNameMap_.count(sn)) return false;
                Option* opt = &options_[shortNameMap_[sn]];
                std::string val = arg.substr(3);
                if (opt->type == ArgType::String) { if (opt->multi) opt->valuesString.push_back(val); else opt->valueString = val; }
                else if (opt->type == ArgType::Int) { try { int v = std::stoi(val); if (opt->multi) opt->valuesInt.push_back(v); else opt->valueInt = v; } catch(...) { return false; } }
                else return false;
                opt->seen = true;
            } else {
                for (size_t k = 1; k < arg.size(); ++k) {
                    char sn = arg[k];
                    if (!shortNameMap_.count(sn)) return false;
                    Option* opt = &options_[shortNameMap_[sn]];
                    if (opt->type == ArgType::Flag) { opt->valueBool = true; opt->seen = true; }
                    else return false;
                }
            }
        } else {
            if (!positionalOpt) return false;
            Option* opt = positionalOpt;
            if (opt->type == ArgType::String) { if (opt->multi) opt->valuesString.push_back(arg); else opt->valueString = arg; }
            else if (opt->type == ArgType::Int) { try { int v = std::stoi(arg); if (opt->multi) opt->valuesInt.push_back(v); else opt->valueInt = v; } catch(...) { return false; } }
            else return false;
            opt->seen = true;
        }
    }

    if (helpRequested_) return true;

    for (auto& opt : options_) {
        if (opt.type == ArgType::String) {
            if (!opt.multi) {
                if (!opt.seen) {
                    if (opt.hasDefault) opt.valueString = opt.defaultString;
                    else return false;
                }
            } else if (opt.valuesString.size() < opt.minCount) return false;
            if (opt.storeString) *opt.storeString = opt.valueString;
            if (opt.storeStrings) *opt.storeStrings = opt.valuesString;
        } else if (opt.type == ArgType::Int) {
            if (!opt.multi) {
                if (!opt.seen) {
                    if (opt.hasDefault) opt.valueInt = opt.defaultInt;
                    else return false;
                }
            } else if (opt.valuesInt.size() < opt.minCount) return false;
            if (opt.storeInt) *opt.storeInt = opt.valueInt;
            if (opt.storeInts) *opt.storeInts = opt.valuesInt;
        } else if (opt.type == ArgType::Flag) {
            if (!opt.seen && opt.hasDefault) opt.valueBool = opt.defaultBool;
            if (opt.storeBool) *opt.storeBool = opt.valueBool;
        }
    }

    return true;
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    return Parse(args);
}

bool ArgParser::Help() const {
    return helpRequested_;
}

std::string ArgParser::HelpDescription() const {
    std::ostringstream oss;
    oss << programName_ << "\n";
    return oss.str();
}

std::string ArgParser::GetStringValue(const std::string& name) const {
    auto it = longNameMap_.find(name);
    if (it == longNameMap_.end()) return {};
    const Option& opt = options_[it->second];
    if (!opt.multi && !opt.seen && opt.hasDefault) return opt.defaultString;
    if (opt.multi) return opt.valuesString.empty() ? std::string() : opt.valuesString[0];
    return opt.valueString;
}

int ArgParser::GetIntValue(const std::string& name) const {
    auto it = longNameMap_.find(name);
    if (it == longNameMap_.end()) return 0;
    const Option& opt = options_[it->second];
    if (!opt.multi && !opt.seen && opt.hasDefault) return opt.defaultInt;
    if (opt.multi) return opt.valuesInt.empty() ? 0 : opt.valuesInt[0];
    return opt.valueInt;
}

int ArgParser::GetIntValue(const std::string& name, size_t index) const {
    auto it = longNameMap_.find(name);
    if (it == longNameMap_.end()) return 0;
    const Option& opt = options_[it->second];
    return (opt.multi && index < opt.valuesInt.size()) ? opt.valuesInt[index] : 0;
}

bool ArgParser::GetFlag(const std::string& name) const {
    auto it = longNameMap_.find(name);
    if (it == longNameMap_.end()) return false;
    const Option& opt = options_[it->second];
    return opt.seen ? opt.valueBool : (opt.hasDefault ? opt.defaultBool : false);
}

} // namespace ArgumentParser
