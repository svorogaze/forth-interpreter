#include "Executable.h"
#include "Literals.h"
#include <iostream>
#include "StackElement.h"
Operator::Operator(std::string text) : text(text) {
}

Executable::ReturnStatus Operator::Execute(Environment& environment) {
    if (operators_pointers.contains(text)) {
        return operators_pointers[text](environment);
    }
    if (environment.functions.contains(text)) {
        return FunctionCall(environment);
    }
    if (environment.variables.contains(text)) {
        return VariableUse(environment);
    }
    if (IsLiteral(text)) {
        return Literal(environment);
    }
    throw std::runtime_error("unknown operator passed");
}

Executable::ReturnStatus Operator::FunctionCall(Environment& environment) {
    auto status = environment.functions[text]->Execute(environment);
    if (status == ReturnStatus::kLeaveFunction) {
        status = ReturnStatus::kSuccess;
    }
    return status;
}

Executable::ReturnStatus Operator::VariableUse(Environment &environment) {
    environment.PushOnStack(StackElement(reinterpret_cast<int64_t>(environment.variables[text])));
    return ReturnStatus::kSuccess;
}

Executable::ReturnStatus Operator::Literal(Environment &environment) {
    if (IsInteger(text)) {
        environment.PushOnStack(StackElement(std::stoll(text)));
        return ReturnStatus::kSuccess;
    }
    if (IsDouble(text)) {
        environment.PushOnStack(StackElement(std::stod(text)));
        return ReturnStatus::kSuccess;
    }
    environment.PushOnStack(StackElement(reinterpret_cast<int64_t>(text.c_str() + 2)));
    environment.PushOnStack(StackElement(static_cast<int64_t>(text.size() - 3)));
    return ReturnStatus::kSuccess;
}

Executable::ReturnStatus AdditionOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a + b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus SubtractionOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(b - a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus MultiplicationOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a * b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus DivisionOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(b / a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus ModulusOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(b % a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus ConcatenationOperator(Environment& environment) {
    auto sz2 = environment.PopStack().Convert<int64_t>();
    auto address2 = environment.PopStack().Convert<char*>();
    auto sz1 = environment.PopStack().Convert<int64_t >();
    auto address1 = environment.PopStack().Convert<char*>();
    auto res_sz = sz1 + sz2;
    char* res = new char[res_sz];
    memcpy(res, address1, sz1);
    memcpy(res + sz1, address2, sz2);
    environment.PushOnStack((int64_t)res);
    environment.PushOnStack(res_sz);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus NegationOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    environment.PushOnStack(-a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus InversionOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    environment.PushOnStack(~a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus LshiftOperator(Environment& environment) {
    auto k = environment.PopStack();
    auto a = environment.PopStack();
    a.value = (a.Convert<int64_t>() << k.Convert<int64_t>());
    environment.PushOnStack(a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus RshiftOperator(Environment& environment) {
    auto k = environment.PopStack();
    auto a = environment.PopStack();
    a.value = (a.Convert<int64_t>() >> k.Convert<int64_t>());
    environment.PushOnStack(a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus AndOperator(Environment& environment) {
    auto a = environment.PopStack();
    auto b = environment.PopStack();
    environment.PushOnStack(a & b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus XorOperator(Environment& environment) {
    auto a = environment.PopStack();
    auto b = environment.PopStack();
    environment.PushOnStack(a ^ b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus OrOperator(Environment& environment) {
    auto a = environment.PopStack();
    auto b = environment.PopStack();
    environment.PushOnStack(a | b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus NotOperator(Environment& environment) {
    auto a = environment.PopStack();
    environment.PushOnStack(!a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus DupOperator(Environment& environment) {
    auto a = environment.PopStack();
    environment.PushOnStack(a);
    environment.PushOnStack(a);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus DropOperator(Environment& environment) {
    environment.PopStack();
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus SwapOperator(Environment& environment) {
    auto w2 = environment.PopStack();
    auto w1 = environment.PopStack();
    environment.PushOnStack(w2);
    environment.PushOnStack(w1);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus OverOperator(Environment& environment) {
    auto w2 = environment.PopStack();
    auto w1 = environment.PopStack();
    environment.PushOnStack(w1);
    environment.PushOnStack(w2);
    environment.PushOnStack(w1);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus RotOperator(Environment& environment) {
    auto w3 = environment.PopStack();
    auto w2 = environment.PopStack();
    auto w1 = environment.PopStack();
    environment.PushOnStack(w2);
    environment.PushOnStack(w3);
    environment.PushOnStack(w1);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus PickOperator(Environment& environment) {
    auto a = environment.PopStack().Convert<int64_t>();
    if (a >= 0 && a < (int64_t)environment.stack.size()) {
        environment.PushOnStack(
                environment.stack[environment.stack.size() - a - 1]);
        return Executable::ReturnStatus::kSuccess;
    }
    throw std::runtime_error("Incorrect argument");
}

Executable::ReturnStatus NipOperator(Environment& environment) {
    auto w2 = environment.PopStack();
    auto w1 = environment.PopStack();
    environment.PushOnStack(w2);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus TuckOperator(Environment& environment) {
    auto w2 = environment.PopStack();
    auto w1 = environment.PopStack();
    environment.PushOnStack(w2);
    environment.PushOnStack(w1);
    environment.PushOnStack(w2);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus EqualsOperator(Environment& environment) {
    auto a = environment.PopStack();
    auto b = environment.PopStack();
    environment.PushOnStack(a == b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus EqualsStringOperator(Environment& environment) {
    auto len1 = environment.PopStack().Convert<int64_t>();
    auto cdata1 = environment.PopStack().Convert<char*>();
    auto len2 = environment.PopStack().Convert<int64_t>();
    auto cdata2 = environment.PopStack().Convert<char*>();
    environment.PushOnStack(std::string(cdata1, len1) == std::string(cdata2, len2));
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus LessOperator(Environment& environment) {
    auto b = environment.PopStack();
    auto a = environment.PopStack();
    environment.PushOnStack(a < b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus LessEqOperator(Environment& environment) {
    auto b = environment.PopStack();
    auto a = environment.PopStack();
    environment.PushOnStack(a <= b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus GreaterOperator(Environment& environment) {
    auto b = environment.PopStack();
    auto a = environment.PopStack();
    environment.PushOnStack(a > b);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus GreaterEqOperator(Environment& environment)  {
    auto b = environment.PopStack();
    auto a = environment.PopStack();
    environment.PushOnStack(a >= b);
    return Executable::ReturnStatus::kSuccess;
}

template<typename T>
Executable::ReturnStatus AssignmentOperator(Environment& environment) {
    auto ptr = environment.PopStack().Convert<T*>();
    auto val = environment.PopStack().Convert<T>();
    *ptr = val;
    return Executable::ReturnStatus::kSuccess;
}

template<typename T>
Executable::ReturnStatus DereferenceOperator(Environment& environment) {
    auto ptr = environment.PopStack().Convert<T*>();
    environment.PushOnStack(*ptr);
    return Executable::ReturnStatus::kSuccess;
}

template<typename T>
Executable::ReturnStatus InputOperator(Environment& environment) {
    T x;
    std::cin >> x;
    environment.PushOnStack(x);
    return Executable::ReturnStatus::kSuccess;
}

template<>
Executable::ReturnStatus InputOperator<std::string>(Environment& environment) {
    std::string s;
    std::cin >> s;
    char* cs = new char[s.size()];
    environment.PushOnStack((int64_t)cs);
    environment.PushOnStack((int64_t)s.size());
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus StringOutputOperator(Environment& environment) {
    auto sz = environment.PopStack().Convert<size_t>();
    auto address = environment.PopStack().Convert<char*>();
    std::cout << std::string(address, address + sz);
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus CharOutputOperator(Environment& environment) {
    char e = environment.PopStack().Convert<char>();
    std::cout << e;
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus StackBackOutputOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    std::cout << a << ' ';
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus AllStackOutputOperator(Environment& environment) {
    for (auto el : environment.stack) {
        std::cout << el << ' ';
    }
    std::cout << "<" << environment.stack.size() << ">\n";
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus BreakOperator(Environment& environment) {
    return Executable::ReturnStatus::kLeaveLoop;
}

Executable::ReturnStatus ContinueOperator(Environment& environment) {
    return Executable::ReturnStatus::kContinueLoop;
}

Executable::ReturnStatus ReturnOperator(Environment& environment) {
    return Executable::ReturnStatus::kLeaveFunction;
}

Executable::ReturnStatus ToCellOperator(Environment& environment) {
    environment.PushOnStack(environment.PopStack().Convert<int64_t>());
    return Executable::ReturnStatus::kSuccess;
}

Executable::ReturnStatus ToFloatOperator(Environment& environment) {
    environment.PushOnStack(environment.PopStack().Convert<double>());
    return Executable::ReturnStatus::kSuccess;
}

std::map<
    std::string,
    std::function<Executable::ReturnStatus (Environment&)>
> Operator::operators_pointers = {
    {"+", AdditionOperator},
    {"-", SubtractionOperator},
    {"*", MultiplicationOperator},
    {"/", DivisionOperator},
    {"%", ModulusOperator},
    {"s+", ConcatenationOperator},
    {"negate", NegationOperator},
    {"inverse", InversionOperator},
    {"lshift", LshiftOperator},
    {"rshift", RshiftOperator},
    {"and", AndOperator},
    {"or", OrOperator},
    {"xor", XorOperator},
    {"not", NotOperator},
    {"dup", DupOperator},
    {"drop", DropOperator},
    {"swap", SwapOperator},
    {"over", OverOperator},
    {"rot", RotOperator},
    {"pick", PickOperator},
    {"nip", NipOperator},
    {"tuck", TuckOperator},
    {"=", EqualsOperator},
    {"s=", EqualsStringOperator},
    {"<", LessOperator},
    {"<=", LessEqOperator},
    {">", GreaterOperator},
    {">=", GreaterEqOperator},
    {"!", AssignmentOperator<int64_t>},
    {"f!", AssignmentOperator<double>},
    {"c!", AssignmentOperator<char>},
    {"@", DereferenceOperator<int64_t>},
    {"f@", DereferenceOperator<double>},
    {"c@", DereferenceOperator<char>},
    {"sinput", InputOperator<std::string>},
    {"finput", InputOperator<double>},
    {"input", InputOperator<int64_t>},
    {"type", StringOutputOperator},
    {"emit", CharOutputOperator},
    {".", StackBackOutputOperator},
    {".s", AllStackOutputOperator},
    {"leave", BreakOperator},
    {"continue", ContinueOperator},
    {"return", ReturnOperator},
    {"tocell", ToCellOperator},
    {"tofloat", ToFloatOperator},
};