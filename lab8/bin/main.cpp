#include <iostream>
#include <vector>
#include <map>
#include "lib/lazy_adapters.h"

int main() {
    using namespace lazy;

    // Пример 1: фильтрация + трансформация
    std::vector<int> v = {1,2,3,4,5,6};
    std::cout << "filter | transform: ";
    for (auto i : v 
        | filter([](int x){ return x % 2; })
        | transform([](int x){ return x * x; }))
    {
        std::cout << i << ' ';
    }
    std::cout << "\n";

    // Пример 2: take / drop
    std::vector<char> letters = {'a','b','c','d','e','f'};
    std::cout << "drop 2 | take 3: ";
    for (auto c : letters 
        | drop(2) 
        | take(3))
    {
        std::cout << c << ' ';
    }
    std::cout << "\n";

    // Пример 3: reverse
    std::cout << "reverse: ";
    for (auto x : letters | reverse()) {
        std::cout << x << ' ';
    }
    std::cout << "\n";

    // Пример 4: keys / values
    std::map<std::string,int> m = {{"one",1}, {"two",2}, {"three",3}};
    std::cout << "keys: ";
    for (auto& k : keys(m)) {
        std::cout << k << ' ';
    }
    std::cout << "\n";
    std::cout << "values: ";
    for (auto v : values(m)) {
        std::cout << v << ' ';
    }
    std::cout << "\n";

    // Пример 5: сложная цепочка
    std::cout << "complex pipeline: ";
    for (auto x : v 
        | drop(1) 
        | take(4) 
        | filter([](int x){ return x % 2 == 0; })
        | transform([](int x){ return x + 100; })
        | reverse())
    {
        std::cout << x << ' ';
    }
    std::cout << "\n";

    return 0;
}
