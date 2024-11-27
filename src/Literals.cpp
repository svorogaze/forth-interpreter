#include <string>
#include <regex>
#include "Literals.h"

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

