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
        kControlFlowConstruct,
        kFunctionDefinitionStart,
        kFunctionDefinitionEnd
    };
    int row, column;
    LexemeType type;
    std::string text;
private:

};

#endif //LEXEME_H
