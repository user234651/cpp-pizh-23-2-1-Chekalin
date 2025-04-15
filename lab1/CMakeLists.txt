cmake_minimum_required(VERSION 3.10)

# Название проекта
project(word_count)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создание исполняемого файла
add_executable(
    WordCount
    src/main.cpp
    src/word_count.cpp
)

# Подключение директории с заголовками
target_include_directories(WordCount PRIVATE src)

# Указание о том, что исполняемый файл должен собираться в корень проекта
set_target_properties(WordCount PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
