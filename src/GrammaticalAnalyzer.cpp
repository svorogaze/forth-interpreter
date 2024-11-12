#include "GrammaticalAnalyzer.h"

#include <float.h>
#include <stdexcept>
#include <iostream>
GrammaticalAnalyzer::GrammaticalAnalyzer(std::vector<Lexeme> lexemes, std::vector<std::string> code_block_enders_) : lexemes(lexemes) {
    for (auto s : code_block_enders_) {
        code_block_enders.insert(s);
    }
}

Lexeme GrammaticalAnalyzer::GetCurrentLexeme() {
    if (current_lexeme_index >= lexemes.size()) {
        // construct dummy lexeme so all ifs fail and we throw exception
        Lexeme dummy;
        dummy.text = "END OF FILE";
        dummy.type = Lexeme::LexemeType::kError;
        dummy.row = lexemes.back().row;
        dummy.column = lexemes.back().column;
        return dummy;
    }
    return lexemes[current_lexeme_index];
}

void GrammaticalAnalyzer::NextLexeme() {
    current_lexeme_index++;
}

bool GrammaticalAnalyzer::IsFished() {
    return current_lexeme_index >= lexemes.size();
}

void GrammaticalAnalyzer::ThrowException(std::string expected) {
    auto l = GetCurrentLexeme();
    std::string exception_text = "Expected: " + expected + "\nGot: " + l.text
    + "\nat " + std::to_string(l.row) + " row " + std::to_string(l.column) + " column\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::Program() {
    while (!IsFished()) {
        if (GetCurrentLexeme().text == ":") {
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
        code_block_enders.find(GetCurrentLexeme().text) == code_block_enders.end()) {
        if (GetCurrentLexeme().type == Lexeme::LexemeType::kKeyword) {
            ControlFlowConstruct();
        } else {
            Statement();
        }
    }
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
    Statements();
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

void GrammaticalAnalyzer::Statements() {
    while (Statement()) {

    }
}

bool GrammaticalAnalyzer::Statement() {
    if (GetCurrentLexeme().text == "VARIABLE") {
        VariableDefinition();
        return true;
    }
    if (GetCurrentLexeme().text == "CREATE") {
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
    if (GetCurrentLexeme().text != "cells" &&
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

void GrammaticalAnalyzer::Analyze() {
    try {
        Program();
    } catch (std::exception& e) {
        std::cout << "Syntax error:\n"  << e.what();
        exit(0);
    }
}
