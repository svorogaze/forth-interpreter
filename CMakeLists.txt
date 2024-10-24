cmake_minimum_required(VERSION 3.28)
project(forth_interpretator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(forth_interpretator src/main.cpp
        src/Lexeme.cpp
        src/Lexeme.h
        src/Preprocessor.cpp
        src/Preprocessor.h
        src/Parser.cpp
        src/Parser.h
)
