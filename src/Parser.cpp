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
    for (int i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c == ' ' || cur_str == " ") {
            Lexeme current;
            current.position = i - cur_str.size();
            current.type = Lexeme::LexemeType::kError;
            current.text = cur_str;
            if (keyword_trie.Contains(cur_str)) {
                current.type = Lexeme::LexemeType::kKeyword;
            } else if (cur_str == " ") {
                current.type = Lexeme::LexemeType::kWhitespace;
            } else if (IsLiteral(cur_str)) {
                current.type = Lexeme::LexemeType::kLiteral;
            } else if (operator_trie.Contains(cur_str)) {
                current.type = Lexeme::LexemeType::kOperator;
            } else {
                current.type = Lexeme::LexemeType::kIdentifier;
            }
            result.push_back(current);
            cur_str.clear();
        }
        cur_str += c;
    }
    Trie defined_identifiers;
    bool defining_identifier = false;
    for (auto lexeme : result) {
        if (lexeme.type == Lexeme::LexemeType::kIdentifier && !defined_identifiers.Contains(lexeme.text)) {
            if (!defining_identifier) {
                lexeme.type = Lexeme::LexemeType::kError;
            } else {
                defined_identifiers.Add(lexeme.text);
                defining_identifier = false;
            }
        } else if (lexeme.type == Lexeme::LexemeType::kKeyword && (lexeme.text == ":" || lexeme.text == "CREATE")) {
            defining_identifier = true;
        } else if (defining_identifier && lexeme.type != Lexeme::LexemeType::kWhitespace) {
            // we don't want non-identifiers after we started defining new one
            lexeme.type = Lexeme::LexemeType::kError;
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

