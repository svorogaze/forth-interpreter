#include "Preprocessor.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

Preprocessor::Preprocessor(std::string file_path) {
    std::ifstream code_file(file_path);
    if (!code_file.is_open()) {
        throw std::logic_error("failed to open file for preprocessing");
    }
    std::string line;
    while (std::getline(code_file, line)) {
        line += '\n';
        current_text += line;
    }
}

std::string Preprocessor::GetCurrentText() {
    return current_text;
}

void Preprocessor::ToOneLine() {
    std::replace_if(current_text.begin(), current_text.end(), [](auto c) {
        return static_cast<int>(c) < 32;
    }, ' ');
}

void Preprocessor::RemoveComments() {
    // braces are for comments in forth
    // so if character is inside at least one pair of braces it is irrevelant
    // also \ comments to new line so we remove it too
    int balance = 0;
    bool slash_comment = false;
    std::string result;
    for (auto c : current_text) {
        if (c == '\\') {
            slash_comment = true;
        }
        if (c == '\n') {
            slash_comment = false;
        }
        if (c == '(') {
            balance++;
        }
        if (balance == 0 && !slash_comment) {
            result += c;
        }
        if (c == ')') {
            balance--;
        }
    }

    current_text = result;
}




