#ifndef GRAMMATICALANALYZER_H
#define GRAMMATICALANALYZER_H
#include "Lexeme.h"
#include <vector>
#include <set>
class GrammaticalAnalyzer {
public:
    GrammaticalAnalyzer(const std::vector<Lexeme>&, const std::vector<std::string>&);
    void Analyze();
private:
    Lexeme GetCurrentLexeme();
    void NextLexeme();
    // exceptions
    void ThrowSyntaxException(const std::string&);
    void ThrowUndefinedException(const Lexeme&);
    void ThrowNotNumberException(const Lexeme&);
    void ThrowFindCycle(const Lexeme&);
    void ThrowFindFunction(const Lexeme&);
    void ThrowRedefinition(const Lexeme&);
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
    bool in_cycle = false;
    int in_function = 0;
};



#endif
