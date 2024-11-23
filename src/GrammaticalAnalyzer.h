#ifndef GRAMMATICALANALYZER_H
#define GRAMMATICALANALYZER_H
#include "Lexeme.h"
#include <vector>
#include <set>
class GrammaticalAnalyzer {
public:
    GrammaticalAnalyzer(const std::vector<Lexeme>& lexemes, const std::vector<std::string>& code_block_enders);
    void Analyze();
private:
    Lexeme GetCurrentLexeme();
    void NextLexeme();
    // exceptions
    void ThrowSyntaxException(const std::string&);
    void ThrowGenericException(const Lexeme& l, std::string prefix_text = "", std::string suffix_text = "");
    void ThrowUndefinedException(const Lexeme& l);
    void ThrowNotIntegerException(const Lexeme& l);
    void ThrowNotInLoopException(const Lexeme& l);
    void ThrowNotInFunctionException(const Lexeme& l);
    void ThrowRedefinitionException(const Lexeme& l);
    bool IsFished();
    bool IsInteger(const std::string& text);
    // Grammar of language
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
    std::vector<Lexeme> lexemes_;
    int current_lexeme_index_ = 0;
    std::set<std::string> code_block_enders_;
    std::set<std::string> defined_identifiers;
    int loop_counter = 0;
    int function_counter = 0;
};



#endif
