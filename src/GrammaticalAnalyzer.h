/**
 * @file GrammaticalAnalyzer.h
 * @brief Defines the GrammaticalAnalyzer class for performing syntactic analysis.
 */

#ifndef GRAMMATICALANALYZER_H
#define GRAMMATICALANALYZER_H

#include "Lexeme.h"
#include "Environment.h"
#include <vector>
#include <set>
#include <string>
#include <memory>

/**
 * @class GrammaticalAnalyzer
 * @brief Performs syntactic analysis and generates the resulting environment.
 */
class GrammaticalAnalyzer {
public:
    /**
     * @brief Constructs a GrammaticalAnalyzer.
     * @param lexemes A vector of lexemes to analyze.
     * @param code_block_enders A set of keywords that signify the end of a code block.
     */
    GrammaticalAnalyzer(const std::vector<Lexeme>& lexemes, const std::vector<std::string>& code_block_enders);

    /**
     * @brief Performs grammatical analysis on the provided lexemes.
     */
    void Analyze();

    /**
     * @brief The resulting environment after analysis.
     */
    Environment resulting_environment;

private:
    /**
     * @brief Retrieves the current lexeme being analyzed.
     * @return The current lexeme.
     */
    Lexeme GetCurrentLexeme();

    /**
     * @brief Advances to the next lexeme in the analysis.
     */
    void NextLexeme();

    /**
     * @brief Throws a syntax error exception with a specific message.
     * @param message The error message.
     */
    void ThrowSyntaxException(const std::string& message);

    /**
     * @brief Throws a generic exception for a given lexeme.
     * @param l The lexeme where the error occurred.
     * @param prefix_text Text to prepend to the error message.
     * @param suffix_text Text to append to the error message.
     */
    void ThrowGenericException(const Lexeme &l, const std::string& prefix_text, const std::string& suffix_text);

    /**
     * @brief Throws an exception for an undefined identifier.
     * @param l The lexeme representing the undefined identifier.
     */
    void ThrowUndefinedException(const Lexeme& l);

    /**
     * @brief Throws an exception for a non-integer value.
     * @param l The lexeme representing the non-integer value.
     */
    void ThrowNotIntegerException(const Lexeme& l);

    /**
     * @brief Throws an exception when a loop-related operation is outside a loop.
     * @param l The lexeme causing the error.
     */
    void ThrowNotInLoopException(const Lexeme& l);

    /**
     * @brief Throws an exception when a function-related operation is outside a function.
     * @param l The lexeme causing the error.
     */
    void ThrowNotInFunctionException(const Lexeme& l);

    /**
     * @brief Throws an exception for redefinition of an identifier.
     * @param l The lexeme representing the redefined identifier.
     */
    void ThrowRedefinitionException(const Lexeme& l);

    /**
     * @brief Checks if the analysis has reached the end of the lexemes.
     * @return True if all lexemes have been processed, false otherwise.
     */
    bool IsFished();

    /**
     * @brief Parses the program grammar.
     */
    void Program();

    /**
     * @brief Parses a function definition.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> FunctionDefinition();

    /**
     * @brief Parses a code block.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> CodeBlock();

    /**
     * @brief Parses control flow constructs such as if, while, and for loops.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> ControlFlowConstruct();

    /**
     * @brief Parses a while loop.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> While();

    /**
     * @brief Parses a for loop.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> For();

    /**
     * @brief Parses an if-else construct.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> If();

    /**
     * @brief Parses a switch statement.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> Switch();

    /**
     * @brief Parses a series of statements.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> Statements();

    /**
     * @brief Parses a single statement.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> Statement();

    /**
     * @brief Parses a variable definition.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> VariableDefinition();

    /**
     * @brief Parses an array definition.
     * @return A shared pointer to the parsed Executable.
     */
    std::shared_ptr<Executable> ArrayDefinition();

    /**
     * @brief Processes size operators.
     */
    void SizeOperators();

    std::vector<Lexeme> lexemes_; ///< The list of lexemes to analyze.
    int current_lexeme_index_ = 0; ///< The current index in the lexemes vector.
    std::set<std::string> code_block_enders_; ///< The set of keywords that signify the end of a code block.
    std::set<std::string> defined_identifiers; ///< The set of currently defined identifiers.
    int loop_counter = 0; ///< Tracks the current nesting level of loops.
    int function_counter = 0; ///< Tracks the current nesting level of functions.
};

#endif // GRAMMATICALANALYZER_H
