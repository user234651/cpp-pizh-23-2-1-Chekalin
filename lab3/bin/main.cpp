#include "../lib/Sandpile.h"
#include <iostream>
#include <filesystem>

int main() {
    try {
        // Задаём путь к входному файлу
        std::string inputFile = "input/main_input.tsv";

        // Проверяем, существует ли входной файл
        if (!std::filesystem::exists(inputFile)) {
            std::cerr << "Файл " << inputFile << " не найден.\n";
            return 1;
        }

        // Создаём модель песчаной кучи размером 100x100
        Sandpile pile(100, 100);

        // Загружаем точки из файла
        pile.loadFromFile(inputFile);

        // Запускаем симуляцию:
        // максимум 100000 итераций, промежуточные результаты не сохраняются (freq = 0)
        // имя входного файла используется для создания BMP-файла с результатом
        pile.run(100000, 0, inputFile);

        // Выводим сообщение об успешном завершении
        std::cout << "Симуляция завершена. Результат сохранён в main_input.bmp\n";
    } catch (const std::exception& e) {
        // В случае ошибки выводим её описание
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
