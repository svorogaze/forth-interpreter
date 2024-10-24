cmake_minimum_required(VERSION 3.28)
project(forth_interpretator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(forth_interpretator main.cpp
        Lexeme.cpp
        Lexeme.h
        Preprocessor.cpp
        Preprocessor.h
        Parser.cpp
        Parser.h
)
