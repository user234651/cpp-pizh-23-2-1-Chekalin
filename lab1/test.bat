@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo === Тест 1: Подсчет слов ===
WordCount.exe "someText.txt" -w

echo.
echo === Тест 2: Подсчет строк ===
WordCount.exe "someText.txt" -l

echo.
echo === Тест 3: Подсчет символов ===
WordCount.exe "someText.txt" -c

echo.
echo === Тест 4: Комбинирование флагов: слова, строки, символы ===
WordCount.exe "someText.txt" -w -l -c

echo.
echo === Тест 5: Все три флага, нет кавычек ===
WordCount.exe someText.txt -w -l -c

echo.
echo === Тест 6: Только имя файла ===
WordCount.exe "someText.txt"

echo.
echo === Тест 7: Только параметры ===
WordCount.exe -w -l

echo.
echo === Тест 8: Запуск без параметров ===
WordCount.exe

echo.
echo === Тест 9: Неверный путь к файлу ===
WordCount.exe "not_existing_file.txt" -w

echo.
echo === Тест 10: Случайный порядок аргументов ===
WordCount.exe -l "someText.txt" -c -w

echo.
pause
