/**
 * @file Parser.h
 * @brief Defines the Parser class for lexical analysis of input strings.
 */

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Lexeme.h"

/**
 * @class Parser
 * @brief Performs lexical analysis of input strings, producing a vector of lexemes.
 */
class Parser {
public:
    /**
     * @brief Constructs a Parser instance.
     * @param input The input string to parse.
     * @param keywords A list of keywords to recognize.
     * @param operators A list of operators to recognize.
     */
    explicit Parser(const std::string& input,
                    const std::vector<std::string>& keywords,
                    const std::vector<std::string>& operators);

    /**
     * @brief Retrieves the result of the parsing operation.
     * @return A vector of lexemes generated from the input string.
     */
    std::vector<Lexeme> GetResult();

private:
    /**
     * @struct Trie
     * @brief A simple trie structure for keyword and operator matching.
     *
     * While its use in this project is debatable, it is implemented for demonstration purposes.
     */
    struct Trie {
        /**
         * @struct Node
         * @brief Represents a single node in the Trie.
         */
        struct Node {
            bool is_terminal = false; ///< Indicates whether this node represents the end of a valid string.
            std::map<char, std::unique_ptr<Node>> go; ///< Map of child nodes indexed by characters.
        };

        std::unique_ptr<Node> root = std::make_unique<Node>(); ///< The root node of the Trie.

        /**
         * @brief Adds a string to the Trie.
         * @param str The string to add.
         */
        void Add(std::string str);

        /**
         * @brief Checks if a string exists in the Trie.
         * @param str The string to check.
         * @return True if the string exists, false otherwise.
         */
        bool Contains(std::string str);
    };

    std::vector<Lexeme> result; ///< Stores the parsed lexemes.
};

#endif // PARSER_H
