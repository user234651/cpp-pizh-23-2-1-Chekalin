cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Lab1)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создание исполняемого файла
add_executable(
    Program
    lib/main.cpp
    lib/App.cpp
    lib/User.cpp
)

# Подключение директории с заголовками
target_include_directories(Program PRIVATE src)

# Указание о том, что исполняемый файл должен собираться в корень проекта
set_target_properties(Program PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
