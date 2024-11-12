#include "Parser.h"
#include <regex>
#include "Lexeme.h"
std::vector<Lexeme> Parser::GetResult() {
    return result;
}

bool IsInteger(const std::string& str) {
    return std::regex_match(str, std::regex("-?[0-9]+"));
}

bool IsDouble(const std::string& str) {
    return std::regex_match(str, std::regex("-?[0-9]+([\\.][0-9]+)?"));
}

bool IsString(const std::string& str) {
    return str.size() >= 3 && str[0] == 's' && str[1] == '"' && str.back() == '"';
}

bool IsLiteral(const std::string& str) {
    return IsInteger(str) || IsDouble(str) || IsString(str);
}

bool IsDelimeter(char c) {
    return c == '\n' || c == ' ';
}

Parser::Parser(const std::string& input, const std::vector<std::string>& keywords, const std::vector<std::string>& operators) {
    Trie keyword_trie;
    for (const auto& str : keywords) {
        keyword_trie.Add(str);
    }
    Trie operator_trie;
    for (const auto& str : operators) {
        operator_trie.Add(str);
    }
    std::string cur_str;
    int line = 1;
    int current_column = 0;
    for (int i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (IsDelimeter(c) && !cur_str.empty()) {
            Lexeme current;
            current.row = line;
            current.column = current_column;
            current.type = Lexeme::LexemeType::kError;
            current.text = cur_str;
            if (keyword_trie.Contains(cur_str)) {
                current.type = Lexeme::LexemeType::kKeyword;
            } else if (IsLiteral(cur_str)) {
                current.type = Lexeme::LexemeType::kLiteral;
            } else if (operator_trie.Contains(cur_str)) {
                current.type = Lexeme::LexemeType::kOperator;
            } else if (cur_str == ":") {
                current.type = Lexeme::LexemeType::kFunctionDefinitionStart;
            } else if (cur_str == ";") {
                current.type = Lexeme::LexemeType::kFunctionDefinitionEnd;
            } else {
                current.type = Lexeme::LexemeType::kIdentifier;
            }
            result.push_back(current);
            cur_str.clear();
        } else if (!IsDelimeter(c)) {
            cur_str += c;
        }
        if (c == '\n') {
            line++;
            current_column = 0;
        } else {
            current_column++;
        }
    }
}

void Parser::Trie::Add(std::string str) {
    Node* current_node = root.get();
    for (auto c : str) {
        if (current_node->go.find(c) == current_node->go.end()) {
            current_node->go[c] = std::make_unique<Node>();
        }
        current_node = current_node->go[c].get();
    }
    current_node->is_terminal = true;
}

bool Parser::Trie::Contains(std::string str) {
    Node* current_node = root.get();
    for (auto c : str) {
        if (current_node->go.find(c) == current_node->go.end()) {
            return false;
        }
        current_node = current_node->go[c].get();
    }
    return true;
}

