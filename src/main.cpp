#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <csignal>
#include "Executable.h"
#include "GrammaticalAnalyzer.h"
#include "Preprocessor.h"
#include "Parser.h"
#include "Lexeme.h"
#include "StackElement.h"
int main() {
    std::vector<std::string> keywords = {
        "BEGIN",
        "WHILE",
        "REPEAT",
        "DO",
        "LOOP",
        "IF",
        "ENDIF",
        "ELSE",
        "CASE",
        "OF",
        "ENDOF",
        "ENDCASE"
    };

    std::vector<std::string> operators = {
        "dup",
        "drop",
        "swap",
        "over",
        "swap",
        "rot",
        "pick",
        "nip",
        "tuck",
        "roll",
        "+",
        "s+",
        "*",
        "/",
        "-",
        "%",
        "negate",
        "invert",
        "lshift",
        "rshift",
        "<",
        ">",
        "<=",
        ">=",
        "=",
        "s=",
        "and",
        "or",
        "xor",
        "not",
        "!",
        "f!",
        "c!",
        "@",
        "c@",
        "f@",
        "sinput",
        "finput",
        "input",
        "type",
        ".",
        ".s",
        "emit",
        "leave",
        "continue",
        "VARIABLE",
        "CREATE",
        "allot",
        "chars",
        "floats",
        "cells",
        "tofloat",
        "tocell",
        "return"
    };
    /*
    if (argc != 2) {
        throw std::logic_error("number of command line arguments arguments doesn't match");
    }
    */
    const std::string code_file("C:\\Users\\vvzag\\CLionProjects\\forth_interpretator\\test.txt");
    Preprocessor preprocessor(code_file);
    preprocessor.RemoveComments();
    std::string processed_string = preprocessor.GetCurrentText();


    Parser parser(processed_string, keywords, operators);
    auto lexemes = parser.GetResult();
    GrammaticalAnalyzer grammatical_analyzer(lexemes, {";", "REPEAT", "LOOP", "ELSE", "ENDOF", ":", "ENDIF", "WHILE"});
    grammatical_analyzer.Analyze();
    try {
        grammatical_analyzer.resulting_environment.code->Execute(grammatical_analyzer.resulting_environment);
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}
