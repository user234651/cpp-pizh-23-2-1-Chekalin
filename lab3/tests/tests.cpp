#include <gtest/gtest.h>
#include "../lib/Sandpile.h"

// Проверка, что файл успешно загружается без исключений
TEST(SandpileTest, LoadFromFileWorks) {
    Sandpile s(5, 5);
    EXPECT_NO_THROW(s.loadFromFile("input/test_input.tsv"));
}

// Проверка, что метод run не вызывает исключений при выполнении
TEST(SandpileTest, RunDoesNotCrash) {
    Sandpile s(5, 5);
    s.loadFromFile("input/test_input.tsv");
    EXPECT_NO_THROW(s.run());
}

// Проверка, что после выполнения симуляции в поле появились зёрна
TEST(SandpileTest, FieldIsNonEmptyAfterRun) {
    Sandpile s(5, 5);
    s.loadFromFile("input/test_input.tsv");
    s.run();
    const auto& field = s.getField();
    bool hasGrains = false;
    for (const auto& row : field)
        for (auto cell : row)
            if (cell > 0)
                hasGrains = true;
    EXPECT_TRUE(hasGrains);
}

// Проверка, что размеры поля совпадают с заданными при создании
TEST(SandpileTest, FieldSizeIsCorrect) {
    Sandpile s(7, 3);
    s.loadFromFile("input/test_input.tsv");
    s.run();
    const auto& field = s.getField();
    EXPECT_EQ(field.size(), 3); // высота
    for (const auto& row : field)
        EXPECT_EQ(row.size(), 7); // ширина
}

// Проверка, что после завершения симуляции все клетки стали устойчивыми (< 4)
TEST(SandpileTest, StableAfterRun) {
    Sandpile s(10, 10);
    s.loadFromFile("input/test_input.tsv");
    s.run();
    const auto& field = s.getField();
    for (const auto& row : field)
        for (auto cell : row)
            EXPECT_LT(cell, 4);
}
