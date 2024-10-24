#include "Lexeme.h"
#include "stdexcept"


std::string ToString(Lexeme::LexemeType l) {
    if (l == Lexeme::LexemeType::kError) {
        return "error type";
    } else if (l == Lexeme::LexemeType::kIdentifier) {
        return "identifier";
    } else if (l == Lexeme::LexemeType::kKeyword) {
        return "keyword";
    } else if (l == Lexeme::LexemeType::kOperator) {
        return "operator";
    } else if (l == Lexeme::LexemeType::kLiteral) {
        return "literal";
    } else if (l == Lexeme::LexemeType::kWhitespace) {
        return "whitespace";
    }
    throw std::logic_error("Trying to convert unknown lexeme type to string");
}