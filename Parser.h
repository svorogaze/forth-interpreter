#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Lexeme.h"

class Parser {
public:
    explicit Parser(const std::string& input,
        const std::vector<std::string>& keywords,
        const std::vector<std::string>& operators);
    std::vector<Lexeme> GetResult();
private:
    struct Trie { // trie implementation, trust me, I know this is stupid and unnecessary for this project, but I was forced to do it
        struct Node {
            bool is_terminal = false;
            std::map<char, std::unique_ptr<Node>> go;
        };
        std::unique_ptr<Node> root = std::make_unique<Node>();
        void Add(std::string);
        bool Contains(std::string);
    };
private:
    std::vector<Lexeme> result;
};


#endif //PARSER_H
