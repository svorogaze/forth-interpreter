#include "GrammaticalAnalyzer.h"
#include <stdexcept>
#include <iostream>
#include <utility>

// public

void GrammaticalAnalyzer::Analyze() {
    try {
        Program();
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

void GrammaticalAnalyzer::ThrowException(const std::string& expected) {
    auto l = GetCurrentLexeme();
    std::string exception_text = "Expected: " + expected + "\nGot: " + l.text
                                 + "\nat " + std::to_string(l.row) + " row " + std::to_string(l.column) + " column\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::Program() {
    while (!IsFished()) {
        if (GetCurrentLexeme().text == ":") { // forth function
            FunctionDefinition();
        } else {
            CodeBlock();
        }
    }
}

void GrammaticalAnalyzer::FunctionDefinition() {
    if (GetCurrentLexeme().text != ":") {
        ThrowException(":");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowException("identifier");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != ";") {
        ThrowException(";");
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
    if (GetCurrentLexeme().text == "CREATE") { // creat array
        ArrayDefinition();
        return true;
    }
    if (GetCurrentLexeme().type == Lexeme::LexemeType::kOperator) {
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
        While();
    } else if (GetCurrentLexeme().text == "DO") {
        For();
    } else if (GetCurrentLexeme().text == "IF") {
        If();
    } else if (GetCurrentLexeme().text == "CASE") {
        Switch();
    } else {
        ThrowException("Control flow construct");
    }
}

void GrammaticalAnalyzer::If() {
    if (GetCurrentLexeme().text != "IF") {
        ThrowException("IF");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text == "ELSE") {
        NextLexeme();
        CodeBlock();
    }
    if (GetCurrentLexeme().text != "ENDIF") {
        ThrowException("ENDIF");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::For() {
    if (GetCurrentLexeme().text != "DO") {
        ThrowException("DO");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != "LOOP") {
        ThrowException("LOOP");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::Switch() {
    if (GetCurrentLexeme().text != "CASE") {
        ThrowException("CASE");
    }
    NextLexeme();
    while (GetCurrentLexeme().text != "ENDCASE") {
        if (GetCurrentLexeme().type != Lexeme::LexemeType::kLiteral) {
            ThrowException("literal");
        }
        NextLexeme();
        if (GetCurrentLexeme().text != "OF") {
            ThrowException("OF");
        }
        NextLexeme();
        CodeBlock();
        if (GetCurrentLexeme().text != "ENDOF") {
            ThrowException("ENDOF");
        }
        NextLexeme();
    }
    if (GetCurrentLexeme().text != "ENDCASE") {
        ThrowException("ENDCASE");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::While() {
    if (GetCurrentLexeme().text != "BEGIN") {
        ThrowException("BEGIN");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != "WHILE") {
        ThrowException("WHILE");
    }
    NextLexeme();
    CodeBlock();
    if (GetCurrentLexeme().text != "REPEAT") {
        ThrowException("REPEAT");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::VariableDefinition() {
    if (GetCurrentLexeme().text != "VARIABLE") {
        ThrowException("VARIABLE");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowException("identifier");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::SizeOperators() {
    if (GetCurrentLexeme().text != "cells" && // int
        GetCurrentLexeme().text != "floats" &&
        GetCurrentLexeme().text != "chars") {
        ThrowException("size operator");
    }
    NextLexeme();
}

void GrammaticalAnalyzer::ArrayDefinition() {
    if (GetCurrentLexeme().text != "CREATE") {
        ThrowException("CREATE");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowException("identifier");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kLiteral) {
        ThrowException("literal");
    }
    NextLexeme();
    SizeOperators();
    if (GetCurrentLexeme().text != "allot") {
        ThrowException("allot");
    }
    NextLexeme();
}