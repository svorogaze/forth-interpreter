#include <iostream>
#include <stdexcept>
#include <string>

#include "Preprocessor.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        throw std::logic_error("number of command line arguments arguments doesn't match");
    }

    const std::string code_file(argv[1]);
    Preprocessor preprocessor(code_file);
    preprocessor.ToOneLine();

    std::string processed_string = preprocessor.GetCurrentText();

}
