#include "GrammaticalAnalyzer.h"
#include "Executable.h"
#include "Environment.h"
#include "Literals.h"
#include <stdexcept>
#include <iostream>
#include <utility>
#include <regex>

// public

void GrammaticalAnalyzer::Analyze() {
    try {
        defined_identifiers.insert("I"); //special variable for index of most inner for loop
        Program();
        for (auto l: lexemes_) {
            if (l.type == Lexeme::LexemeType::kIdentifier &&
                !defined_identifiers.contains(l.text)) {
                ThrowUndefinedException(l);
            }
        }
    } catch (std::exception &e) {
        std::cout << "Syntax error:\n" << e.what();
        exit(0);
    }
}

GrammaticalAnalyzer::GrammaticalAnalyzer(const std::vector<Lexeme> &_lexemes,
                                         const std::vector<std::string> &_code_block_enders)
    : lexemes_(_lexemes) {
    for (const auto &s: _code_block_enders) {
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

void GrammaticalAnalyzer::ThrowSyntaxException(const std::string &expected) {
    auto l = GetCurrentLexeme();
    std::string exception_text = std::to_string(l.row) + ":"
                                 + std::to_string(l.column) + ": " +
                                 "Expected: " + "'" + expected + "'"
                                 + "\nGot: " + "'" + l.text + "'" + "\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowGenericException(const Lexeme &l, const std::string &prefix_text, const std::string &suffix_text) {
    std::string exception_text = std::to_string(l.row) + ":" +
                                 std::to_string(l.column) + ": "
                                 + prefix_text + "'" + l.text + "'" + suffix_text + "\n";
    throw std::runtime_error(exception_text);
}

void GrammaticalAnalyzer::ThrowUndefinedException(const Lexeme &l) {
    ThrowGenericException(l, "Undefined identifier ", "");
}

void GrammaticalAnalyzer::ThrowNotIntegerException(const Lexeme &l) {
    ThrowGenericException(l, "Literal ", " must be an integer");
}

void GrammaticalAnalyzer::ThrowNotInLoopException(const Lexeme &l) {
    ThrowGenericException(l, "Operator ", " must be in loop");
}

void GrammaticalAnalyzer::ThrowNotInFunctionException(const Lexeme &l) {
    ThrowGenericException(l, "Operator ", " must be in function");
}

void GrammaticalAnalyzer::ThrowRedefinitionException(const Lexeme &l) {
    ThrowGenericException(l, "Redefinition of identifier ", "");
}

void GrammaticalAnalyzer::Program() {
    std::shared_ptr<Codeblock> result(new Codeblock);
    while (!IsFished()) {
        if (GetCurrentLexeme().text == ":") {
            function_counter++;
            FunctionDefinition();
            function_counter--;
        } else {
            auto block = CodeBlock();
            result->statements.push_back(block);
        }
    }
    resulting_environment.code = result;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::FunctionDefinition() {
    if (GetCurrentLexeme().text != ":") {
        ThrowSyntaxException(":");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowSyntaxException("identifier");
    }
    std::string function_name = GetCurrentLexeme().text;
    if (defined_identifiers.contains(function_name)) {
        ThrowRedefinitionException(GetCurrentLexeme());
    }
    defined_identifiers.insert(function_name);
    NextLexeme();
    auto function_body = CodeBlock();
    resulting_environment.functions[function_name] = function_body;
    if (GetCurrentLexeme().text != ";") {
        ThrowSyntaxException(";");
    }
    NextLexeme();
    return function_body;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::CodeBlock() {
    std::shared_ptr<Codeblock> result(new Codeblock);
    while (!IsFished() && !code_block_enders_.contains(GetCurrentLexeme().text)) {
        std::shared_ptr<Executable> block;
        if (GetCurrentLexeme().type == Lexeme::LexemeType::kKeyword) {
            block = ControlFlowConstruct();
        } else {
            block = Statement();
        }
        result->statements.push_back(block);
    }
    return result;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::Statements() {
    std::shared_ptr<Codeblock> result(new Codeblock);
    while (true) {
        auto statement = Statement();
        if (!statement) {
            return result;
        }
        result->statements.push_back(statement);
    }
}

std::shared_ptr<Executable> GrammaticalAnalyzer::Statement() {
    if (GetCurrentLexeme().text == "VARIABLE") {
        return VariableDefinition();
    }
    if (GetCurrentLexeme().text == "CREATE") {
        return ArrayDefinition();
    }
    if (GetCurrentLexeme().type == Lexeme::LexemeType::kOperator) {
        if (GetCurrentLexeme().text == "leave" ||
            GetCurrentLexeme().text == "continue") {
            if (loop_counter == 0) {
                ThrowNotInLoopException(GetCurrentLexeme());
            }
        }
        if (GetCurrentLexeme().text == "return") {
            if (function_counter == 0) {
                ThrowNotInFunctionException(GetCurrentLexeme());
            }
        }
        std::shared_ptr<Operator> result(new Operator(GetCurrentLexeme().text));
        NextLexeme();
        return result;
    }
    if (GetCurrentLexeme().type == Lexeme::LexemeType::kLiteral ||
        GetCurrentLexeme().type == Lexeme::LexemeType::kIdentifier) {
        std::shared_ptr<Operator> result(new Operator(GetCurrentLexeme().text));
        NextLexeme();
        return result;
    }
    return nullptr;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::ControlFlowConstruct() {
    if (GetCurrentLexeme().text == "BEGIN") {
        loop_counter++;
        auto result = While();
        loop_counter--;
        return result;
    } else if (GetCurrentLexeme().text == "DO") {
        loop_counter++;
        auto result = For();
        loop_counter--;
        return result;
    } else if (GetCurrentLexeme().text == "IF") {
        return If();
    } else if (GetCurrentLexeme().text == "CASE") {
        return Switch();
    } else {
        ThrowSyntaxException("Control flow construct");
        return nullptr;
    }
}

std::shared_ptr<Executable> GrammaticalAnalyzer::If() {
    std::shared_ptr<class If> result(new class If);
    if (GetCurrentLexeme().text != "IF") {
        ThrowSyntaxException("IF");
    }
    NextLexeme();
    result->if_part = CodeBlock();
    if (GetCurrentLexeme().text == "ELSE") {
        NextLexeme();
        result->else_part = CodeBlock();
    }
    if (GetCurrentLexeme().text != "ENDIF") {
        ThrowSyntaxException("ENDIF");
    }
    NextLexeme();
    return result;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::For() {
    std::shared_ptr<class For> loop(new class For);
    if (GetCurrentLexeme().text != "DO") {
        ThrowSyntaxException("DO");
    }
    NextLexeme();
    loop->body = CodeBlock();
    if (GetCurrentLexeme().text != "LOOP") {
        ThrowSyntaxException("LOOP");
    }
    NextLexeme();
    return loop;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::While() {
    std::shared_ptr<class While> loop(new class While);
    if (GetCurrentLexeme().text != "BEGIN") {
        ThrowSyntaxException("BEGIN");
    }
    NextLexeme();
    loop->condition = CodeBlock();
    if (GetCurrentLexeme().text != "WHILE") {
        ThrowSyntaxException("WHILE");
    }
    NextLexeme();
    loop->body = CodeBlock();
    if (GetCurrentLexeme().text != "REPEAT") {
        ThrowSyntaxException("REPEAT");
    }
    NextLexeme();
    return loop;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::Switch() {
    std::shared_ptr<class Switch> switch_executable(new class Switch);
    if (GetCurrentLexeme().text != "CASE") {
        ThrowSyntaxException("CASE");
    }
    NextLexeme();
    while (GetCurrentLexeme().text != "ENDCASE") {
        if (GetCurrentLexeme().type != Lexeme::LexemeType::kLiteral) {
            ThrowSyntaxException("literal");
        }
        if (!IsInteger(GetCurrentLexeme().text)) {
            ThrowNotIntegerException(GetCurrentLexeme());
        }
        int64_t literal = std::stoll(GetCurrentLexeme().text);
        NextLexeme();
        if (GetCurrentLexeme().text != "OF") {
            ThrowSyntaxException("OF");
        }
        NextLexeme();
        auto case_code = CodeBlock();
        if (GetCurrentLexeme().text != "ENDOF") {
            ThrowSyntaxException("ENDOF");
        }
        NextLexeme();
        switch_executable->cases[literal] = case_code;
    }
    if (GetCurrentLexeme().text != "ENDCASE") {
        ThrowSyntaxException("ENDCASE");
    }
    NextLexeme();
    return switch_executable;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::VariableDefinition() {
    std::shared_ptr<VariableCreation> result(new VariableCreation);
    if (GetCurrentLexeme().text != "VARIABLE") {
        ThrowSyntaxException("VARIABLE");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowSyntaxException("identifier");
    }
    result->name = GetCurrentLexeme().text;
    if (defined_identifiers.find(GetCurrentLexeme().text) != defined_identifiers.end()) {
        ThrowRedefinitionException(GetCurrentLexeme());
    }
    defined_identifiers.insert(GetCurrentLexeme().text);
    NextLexeme();
    result->size = 1;
    result->type = "cells";
    return result;
}

std::shared_ptr<Executable> GrammaticalAnalyzer::ArrayDefinition() {
    std::shared_ptr<VariableCreation> result(new VariableCreation);
    if (GetCurrentLexeme().text != "CREATE") {
        ThrowSyntaxException("CREATE");
    }
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kIdentifier) {
        ThrowSyntaxException("identifier");
    }
    if (defined_identifiers.contains(GetCurrentLexeme().text)) {
        ThrowRedefinitionException(GetCurrentLexeme());
    }
    result->name = GetCurrentLexeme().text;
    defined_identifiers.insert(GetCurrentLexeme().text);
    NextLexeme();
    if (GetCurrentLexeme().type != Lexeme::LexemeType::kLiteral) {
        ThrowSyntaxException("literal");
    }
    if (!IsInteger(GetCurrentLexeme().text)) {
        ThrowNotIntegerException(GetCurrentLexeme());
    }
    result->size = std::stoll(GetCurrentLexeme().text);
    NextLexeme();
    result->type = GetCurrentLexeme().text;
    SizeOperators();
    if (GetCurrentLexeme().text != "allot") {
        ThrowSyntaxException("allot");
    }
    NextLexeme();
    return result;
}

void GrammaticalAnalyzer::SizeOperators() {
    if (GetCurrentLexeme().text != "cells" && // int
        GetCurrentLexeme().text != "floats" &&
        GetCurrentLexeme().text != "chars") {
        ThrowSyntaxException("size operator");
    }
    NextLexeme();
}
