#ifndef GRAMMATICALANALYZER_H
#define GRAMMATICALANALYZER_H
#include "Lexeme.h"
#include <vector>
#include <set>
class GrammaticalAnalyzer {
public:
    GrammaticalAnalyzer(std::vector<Lexeme> lexemes);
    void Analyze();
private:
    Lexeme GetCurrentLexeme();
    void NextLexeme();
    void ThrowException(std::string expected);
    bool IsFished();
    //Grammar of language
    void Program();
    void FunctionDefinition();
    void CodeBlock();
    void ControlFlowConstruct();
    void While();
    void For();
    void If();
    void Switch();
    void Statements();
    bool Statement();
    void VariableDefinition();
    void ArrayDefinition();
    void SizeOperators();
    std::vector<Lexeme> lexemes;
    int current_lexeme_index = 0;
    std::set<std::string> code_block_enders;
    std::set<std::string> operators;
};



#endif
