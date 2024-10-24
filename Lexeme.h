#ifndef LEXEME_H
#define LEXEME_H
#include <string>


class Lexeme {
public:
    enum class LexemeType { // https://en.wikipedia.org/wiki/Lexical_analysis
        kWhitespace,
        kLiteral,
        kIdentifier,
        kOperator,
        kKeyword,
        kError,
    };
    int position;
    LexemeType type;
    std::string text;
private:

};

std::string ToString(Lexeme::LexemeType type);



#endif //LEXEME_H
