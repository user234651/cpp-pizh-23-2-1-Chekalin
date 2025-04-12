# 🖥️ Лабораторная работа №1: Приложение **WordCount** 📘

## 🧪 Примеры запуска программы:

- Запуск с одним файлом:  
  `WordCount.exe file.txt`

- Подсчёт слов и строк в файле:  
  `WordCount.exe -w -l file.txt`

- Подсчёт строк и символов в файле:  
  `WordCount.exe -l -c file.txt`

- Работа с несколькими файлами:  
  `WordCount.exe file1.txt file2.txt`

- Подсчёт строк, слов и символов в нескольких файлах:  
  `WordCount.exe -l -w -c file1.txt file2.txt`

- Подсчёт строк, слов и байт в нескольких файлах:  
  `WordCount.exe file1.txt file2.txt -l -w -b`

## 🗃️ Структура проекта

```
│   .gitignore
│   CMakeLists.txt
│   RandomText.txt
│   README.md
│   windows_build_test.bat
│   WordCount.exe
│
├───.vscode
│       launch.json
│       tasks.json
│
├───build
├───images
│       activity.uml.png
│       classes.uml.png
│
└───src
        main.cpp
        word_count.cpp
        word_count.h
```

## 📈 Диаграммы

### 🧩 UML-Диаграмма классов
![UML-Диаграмма классов](./images/classes.png)

### 🔁 UML-Диаграмма деятельности
![UML-Диаграмма деятельности](./images/activity.png)

## ⚙️ Сборка приложения на Windows:

1. Установите [MSYS2](https://www.msys2.org/), если он ещё не установлен.

2. Через терминал MSYS2 выполните установку `MinGW` и `CMake`:
   ```bash
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   pacman -S mingw-w64-ucrt-x86_64-cmake
   ```

3. После установки закройте MSYS2 и запустите `PowerShell` или `CMD`, чтобы проверить наличие инструментов:
   ```bash
   g++ --version
   gdb --version
   gcc --version
   cmake --version
   ```

4. Если всё настроено корректно, можно приступить к сборке проекта:

- Перейдите в каталог, содержащий `CMakeLists.txt`, и выполните:
  ```bash
  cmake -S . -B build -G "MinGW Makefiles"
  ```

- После генерации файлов сборки используйте:
  ```bash
  cmake --build build
  ```

- В каталоге появится исполняемый файл `Program.exe`, готовый к запуску.

**Важно**: чтобы избежать проблем с отображением кириллицы в консоли Windows, используйте `windows_demo.bat`. Этот файл запускает `Program.exe` с нужной кодировкой.

Дополнительно можно протестировать работу программы с примером, используя `windows_build_test.bat` и файл `RandomText.txt`.

## 🐧 Сборка и запуск на Linux:

Для Linux-систем процесс сборки проще и не требует установки MinGW.

Выполните в терминале:
```bash
cmake -S . -B build
cmake --build build
```

После сборки исполняемый файл будет доступен в каталоге `build`.

---

📚 **Полезные ссылки**:
- [Инструкция по запуску C++ проектов на Windows](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#links)
- [Инструкция по запуску C++ проектов на Linux](https://code.visualstudio.com/docs/cpp/config-linux)

---