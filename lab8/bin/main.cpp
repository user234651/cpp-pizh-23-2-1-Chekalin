#include <iostream>
#include <vector>
#include <map>
#include "lib/lazy_adapters.h"

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

    // Отбор нечётных и возведение в квадрат
    auto sequence = numbers 
        | pick_if([](int n) { return n % 2 == 1; })
        | apply([](int n) { return n * n; });

    std::cout << "Odd Squares: ";
    for (int n : sequence) {
        std::cout << n << " "; // 1 9 25
    }

    std::cout << "\nFirst 4 items: ";
    for (auto n : numbers | first(4)) {
        std::cout << n << " "; // 1 2 3 4
    }

    std::cout << "\nSkip first 2: ";
    for (auto n : numbers | skip(2)) {
        std::cout << n << " "; // 3 4 5 6
    }

    std::cout << "\nReversed: ";
    for (auto n : numbers | backwards()) {
        std::cout << n << " "; // 6 5 4 3 2 1
    }

    std::cout << "\nReversed + First 3: ";
    for (auto n : numbers | backwards() | first(3)) {
        std::cout << n << " "; // 6 5 4
    }

    std::map<int, std::string> dictionary = {{1, "one"}, {2, "two"}, {3, "three"}};

    std::cout << "\nMap Keys: ";
    for (auto k : dictionary | get_keys()) {
        std::cout << k << " "; // 1 2 3
    }

    std::cout << "\nMap Values: ";
    for (auto val : dictionary | get_values()) {
        std::cout << val << " "; // one two three
    }

    std::cout << "\n";
    return 0;
}
