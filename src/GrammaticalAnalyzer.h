#ifndef GRAMMATICALANALYZER_H
#define GRAMMATICALANALYZER_H
#include "Lexeme.h"
#include "Environment.h"
#include <vector>
#include <set>
#include <string>
#include <memory>
class GrammaticalAnalyzer {
public:
    GrammaticalAnalyzer(const std::vector<Lexeme>& lexemes, const std::vector<std::string>& code_block_enders);
    void Analyze();
    Environment resulting_environment;
private:
    Lexeme GetCurrentLexeme();
    void NextLexeme();
    // exceptions
    void ThrowSyntaxException(const std::string&);
    void ThrowGenericException(const Lexeme &l, const std::string& prefix_text, const std::string& suffix_text);
    void ThrowUndefinedException(const Lexeme& l);
    void ThrowNotIntegerException(const Lexeme& l);
    void ThrowNotInLoopException(const Lexeme& l);
    void ThrowNotInFunctionException(const Lexeme& l);
    void ThrowRedefinitionException(const Lexeme& l);
    bool IsFished();
    bool IsInteger(const std::string& text);
    // Grammar of language
    void Program();
    std::shared_ptr<Executable> FunctionDefinition();
    std::shared_ptr<Executable> CodeBlock();
    std::shared_ptr<Executable> ControlFlowConstruct();
    std::shared_ptr<Executable> While();
    std::shared_ptr<Executable> For();
    std::shared_ptr<Executable> If();
    std::shared_ptr<Executable> Switch();
    std::shared_ptr<Executable> Statements();
    std::shared_ptr<Executable> Statement();
    std::shared_ptr<Executable> VariableDefinition();
    std::shared_ptr<Executable> ArrayDefinition();
    void SizeOperators();
    std::vector<Lexeme> lexemes_;
    int current_lexeme_index_ = 0;
    std::set<std::string> code_block_enders_;
    std::set<std::string> defined_identifiers;
    int loop_counter = 0;
    int function_counter = 0;
};



#endif
