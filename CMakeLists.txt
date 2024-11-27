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
        src/GrammaticalAnalyzer.cpp
        src/GrammaticalAnalyzer.h
        src/StackElement.cpp
        src/StackElement.h
        src/Environment.cpp
        src/Environment.h
        src/Executable.h
        src/While.cpp
        src/Codeblock.cpp
        src/If.cpp
        src/Switch.cpp
        src/VariableCreation.cpp
        src/For.cpp
        src/Operator.cpp
        src/Literals.h
        src/Literals.cpp
)
