#include "GrammaticalAnalyzer.h"
#include <stdexcept>
#include <iostream>
#include <utility>
#include <regex>

// public

void GrammaticalAnalyzer::Analyze() {
    try {
        Program();
        for (auto l : lexemes_) {
            if (l.type == Lexeme::LexemeType::kIdentifier &&
                defined_identifiers.find(l.text) == defined_identifiers.end()) {
                ThrowUndefinedException(l);
            }
        }
    } catch (std::exception& e) { // syntax error
        std::cout << "Syntax error:\n"  << e.what();
        exit(0);
    }
}

GrammaticalAnalyzer::GrammaticalAnalyzer(const std::vector<Lexeme>& _lexemes,
                     const std::vector<std::string>& _code_block_enders)
        : lexemes_(_lexemes) {
    for (const auto& s : _code_block_enders) {
        code_block_enders_.insert(s);
    }
}

// private

Lexeme GrammaticalAnalyzer::GetCurrentLexeme() {
    if (current_lexeme_index_ >= lexemes_.size()) {
        Lexeme lex;
        lex.text = "END OF FILE";
        lex.type = Lexeme::LexemeType::kError;
        lex.row = lexemes_.back().row;
        lex.column = lexemes_.back().column;
        return lex;
    }
    return lexemes_[current_lexeme_index_];
}

void GrammaticalAnalyzer::NextLexeme() {
    ++current_lexeme_index_;
}

bool GrammaticalAnalyzer::IsFished() {
    return current_lexeme_index_ >= lexemes_.size();
}

bool GrammaticalAnalyzer::IsInteger(const std::string &text) {
    return std::regex_match(text, std::regex("-?[0-9]+"));
}

void GrammaticalAnalyzer::ThrowSyntaxException(const std::string& expected) {
    auto l = GetCurrentLexeme();
    std::string exception_text = std::to_string(l.row) + ":"
            + std::to_string(l.column) + ": " +
            "Expected: " + "'" + expected + "'"
            + "\nGot: " + "'" + l.text + "'" + "\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowUndefinedException(const Lexeme& l) {
    std::string exception_text = std::to_string(l.row) + ":" +
            std::to_string(l.column) + ": "
            + "Undefined identifier " + "'" + l.text + "'" + "\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowNotNumberException(const Lexeme& l) {
    std::string exception_text = std::to_string(l.row) + ":"
            + std::to_string(l.column) + ": "
            + "Literal " + "'" + l.text + "'" +  " must be an integer\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowFindCycle(const Lexeme& l) {
    std::string exception_text = std::to_string(l.row) + ":"
            + std::to_string(l.column) + ": "
            + "Operator " + "'" + l.text + "'" + " must be in cycle\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowFindFunction(const Lexeme& l) {
    std::string exception_text = std::to_string(l.row) + ":"
                                 + std::to_string(l.column) + ": "
                                 + "Operator " + "'" + l.text + "'" + " must be in function\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowRedefinition(const Lexeme& l) {
    std::string exception_text = std::to_string(l.row) + ":"
                                 + std::to_string(l.column) + ": "
                                 + "Redefinition of identifier " + "'" + l.text + "'" + "\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::Program() {
    while (!IsFished()) {
        if (GetCurrentLexeme().text == ":") { // forth function
            in_function++;
            FunctionDefinition();
            in_function--;
        } else {
            CodeBlock();
        }
    }
}

void GrammaticalAnalyzer::FunctionDefinition() {
    if (GetCurrentLexeme().text != ":") {
        ThrowSyntaxException(":");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowSyntaxException("identifier");
    }
    defined_identifiers.insert(GetCurrentLexeme().text);
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != ";") {
        ThrowSyntaxException(";");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::CodeBlock() {
    while (!IsFished() &&
           code_block_enders_.find(GetCurrentLexeme().text)
           == code_block_enders_.end()) {
        if (GetCurrentLexeme().type == Lexeme::LexemeType::kKeyword) {
            ControlFlowConstruct();
        } else {
            Statement();
        }
    }
}

void GrammaticalAnalyzer::Statements() {
    while (Statement()) {}
}

bool GrammaticalAnalyzer::Statement() {
    if (GetCurrentLexeme().text == "VARIABLE") { // create var
        VariableDefinition();
        return true;
    }
    if (GetCurrentLexeme().text == "CREATE") { // create array
        ArrayDefinition();
        return true;
    }
    if (GetCurrentLexeme().type == Lexeme::LexemeType::kOperator) {
        if (GetCurrentLexeme().text == "leave" ||
        GetCurrentLexeme().text == "continue") {
            if (!in_cycle) {
                ThrowFindCycle(GetCurrentLexeme());
            }
        }
        if (GetCurrentLexeme().text == "exit") {
            if (!in_function) {
                ThrowFindFunction(GetCurrentLexeme());
            }
        }
        NextLexeme();
        return true;
    }
    if (GetCurrentLexeme().type == Lexeme::LexemeType::kLiteral ||
        GetCurrentLexeme().type == Lexeme::LexemeType::kIdentifier) {
        NextLexeme();
        return true;
    }
    return false;
}

void GrammaticalAnalyzer::ControlFlowConstruct() {
    if (GetCurrentLexeme().text == "BEGIN") {
        in_cycle = true;
        While();
        in_cycle = false;
    } else if (GetCurrentLexeme().text == "DO") {
        in_cycle = true;
        For();
        in_cycle = false;
    } else if (GetCurrentLexeme().text == "IF") {
        If();
    } else if (GetCurrentLexeme().text == "CASE") {
        Switch();
    } else {
        ThrowSyntaxException("Control flow construct");
    }
}

void GrammaticalAnalyzer::If() {
    if (GetCurrentLexeme().text != "IF") {
        ThrowSyntaxException("IF");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text == "ELSE") {
        NextLexeme();
        CodeBlock();
    }
    if (GetCurrentLexeme().text != "ENDIF") {
        ThrowSyntaxException("ENDIF");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::For() {
    if (GetCurrentLexeme().text != "DO") {
        ThrowSyntaxException("DO");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != "LOOP") {
        ThrowSyntaxException("LOOP");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::While() {
    if (GetCurrentLexeme().text != "BEGIN") {
        ThrowSyntaxException("BEGIN");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != "WHILE") {
        ThrowSyntaxException("WHILE");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != "REPEAT") {
        ThrowSyntaxException("REPEAT");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::Switch() {
    // if num
    if (GetCurrentLexeme().text != "CASE") {
        ThrowSyntaxException("CASE");
    }
    NextLexeme();
    while (GetCurrentLexeme().text != "ENDCASE") {
        if (GetCurrentLexeme().type != Lexeme::LexemeType::kLiteral) {
            ThrowSyntaxException("literal");
        }
        if (!IsInteger(GetCurrentLexeme().text)) {
            ThrowNotNumberException(GetCurrentLexeme());
        }
        NextLexeme();
        if (GetCurrentLexeme().text != "OF") {
            ThrowSyntaxException("OF");
        }
        NextLexeme();
        CodeBlock();
        if (GetCurrentLexeme().text != "ENDOF") {
            ThrowSyntaxException("ENDOF");
        }
        NextLexeme();
    }
    if (GetCurrentLexeme().text != "ENDCASE") {
        ThrowSyntaxException("ENDCASE");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::VariableDefinition() {
    if (GetCurrentLexeme().text != "VARIABLE") {
        ThrowSyntaxException("VARIABLE");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowSyntaxException("identifier");
    }
    if (defined_identifiers.find(GetCurrentLexeme().text) != defined_identifiers.end()) {
        ThrowRedefinition(GetCurrentLexeme());
    }
    defined_identifiers.insert(GetCurrentLexeme().text);
    NextLexeme();
}

void GrammaticalAnalyzer::ArrayDefinition() {
    if (GetCurrentLexeme().text != "CREATE") {
        ThrowSyntaxException("CREATE");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowSyntaxException("identifier");
    }
    if (defined_identifiers.find(GetCurrentLexeme().text) != defined_identifiers.end()) {
        ThrowRedefinition(GetCurrentLexeme());
    }
    defined_identifiers.insert(GetCurrentLexeme().text);
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kLiteral) {
        ThrowSyntaxException("literal");
    }
    if (!IsInteger(GetCurrentLexeme().text)) {
        ThrowNotNumberException(GetCurrentLexeme());
    }
    NextLexeme();
    SizeOperators();
    if (GetCurrentLexeme().text != "allot") {
        ThrowSyntaxException("allot");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::SizeOperators() {
    if (GetCurrentLexeme().text != "cells" && // int
        GetCurrentLexeme().text != "floats" &&
        GetCurrentLexeme().text != "chars") {
        ThrowSyntaxException("size operator");
    }
    NextLexeme();
}