#pragma once
#include <string>
#include <vector>
#include <map>

namespace omfl {
    class Section;

    class Value {
    public:
        // Type checks
        bool IsInt() const;
        bool IsFloat() const;
        bool IsBool() const;
        bool IsString() const;
        bool IsArray() const;
        bool IsSection() const;

        // Accessors
        int AsInt() const;
        float AsFloat() const;
        bool AsBool() const;
        const std::string& AsString() const;
        const std::vector<Value>& AsArray() const;
        const Section& AsSection() const;

        // Constructors for internal use
        static Value CreateInt(int v);
        static Value CreateFloat(float v);
        static Value CreateBool(bool v);
        static Value CreateString(const std::string& v);
        static Value CreateArray(const std::vector<Value>& v);
        static Value CreateSection(const Section& sec);

    private:
        enum class Type { INT, FLOAT, BOOL, STRING, ARRAY, SECTION, NONE } type_{Type::NONE};
        int int_{};
        float float_{};
        bool bool_{};
        std::string string_;
        std::vector<Value> array_;
        Section* section_{}; // pointer to owned Section in memory
    };

    class Section {
    public:
        // Get a value (or subsection) by key
        const Value& Get(const std::string& key) const;
        bool Has(const std::string& key) const;

        // Internal storage made public for parser implementation
        std::map<std::string, Value> values_;
    };

    class Config {
    public:
        bool valid = false;
        bool IsValid() const { return valid; }

        const Value& Get(const std::string& key) const;
        bool Has(const std::string& key) const;

        // Root section exposed for parser implementation
        Section root_;
    };

    // Parse an OMFL config from string
    Config Parse(const std::string& str);
}