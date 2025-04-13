#include <lib/number.h>
#include <iostream>

int main() {
    // Проверка вывода большого числа
    uint2022_t value = from_string("12345678901234567890");
    std::cout << value << std::endl;
    return 0;
}
