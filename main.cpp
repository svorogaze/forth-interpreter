#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

#include "Preprocessor.h"
#include "Parser.h"
#include "Lexeme.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        throw std::logic_error("number of command line arguments arguments doesn't match");
    }

    const std::string code_file(argv[1]);
    Preprocessor preprocessor(code_file);
    preprocessor.ToOneLine();
    preprocessor.RemoveComments();
    std::string processed_string = preprocessor.GetCurrentText();

    std::vector<std::string> keywords;
    std::ifstream keywords_file(argv[2]);
    std::string keyword;
    while (keywords_file >> keyword) {
        keywords.push_back(keyword);
    }

    std::vector<std::string> operators = {
        "+",
        "*",
        "/",
        "-",
        "DROP",
        ".s",
        "mod",
        "."
    };

    Parser parser(processed_string, keywords, operators);
    auto lexemes = parser.GetResult();
    std::cout << "Finished!\n";
    for (auto lexeme : lexemes) {
        std::cout << lexeme.text << ' ' << lexeme.position << ' ' << ToString(lexeme.type) << '\n';
    }
}
