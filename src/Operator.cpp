#include "Executable.h"
#include "Literals.h"
#include <iostream>
#include "StackElement.h"
Operator::Operator(std::string text) : text(text) {
}

Executable::ReturnStatus Operator::Execute(Environment& environment) {
    if (operators_pointers.find(text) != operators_pointers.end()) {
        return operators_pointers[text](environment);
    }
    if (environment.functions.find(text) != environment.functions.end()) {
        return FunctionCall(environment);
    }
    if (environment.variables.find(text) != environment.variables.end()) {
        return VariableUse(environment);
    }
    if (IsLiteral(text)) {
        return Literal(environment);
    }
    throw std::runtime_error("unknown operator passed");
}

Executable::ReturnStatus Operator::FunctionCall(Environment& environment) {
    return environment.functions[text]->Execute(environment);
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
    environment.PushOnStack(StackElement(reinterpret_cast<int64_t>(text.c_str())));
    environment.PushOnStack(StackElement(static_cast<int64_t>(text.size())));
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
    environment.PushOnStack(a - b);
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
    environment.PushOnStack(a / b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus ModulusOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a % b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus ConcatenationOperator(Environment& environment) {
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
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus RshiftOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus AndOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a & b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus XorOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a ^ b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus OrOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a | b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus NotOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    environment.PushOnStack(!a);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus DupOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus DropOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus SwapOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus OverOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus RotOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus PickOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus NipOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus TuckOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus EqualsOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a == b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus EqualsStringOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus LessOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a < b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus LessEqOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a <= b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus GreaterOperator(Environment& environment) {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a > b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus GreaterEqOperator(Environment& environment)  {
    StackElement a = environment.PopStack();
    StackElement b = environment.PopStack();
    environment.PushOnStack(a >= b);
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus IntAssignmentOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus FloatAssignmentOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus CharAssignmentOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus IntDereferenceOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus FloatDereferenceOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus CharDereferenceOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus IntInputOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus FloatInputOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus StringInputOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus StringOutputOperator(Environment& environment) {
    return Executable::ReturnStatus::kSuccess;
}
Executable::ReturnStatus CharOutputOperator(Environment& environment) {

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
    return Executable::ReturnStatus::kLeaveLoop;
}
Executable::ReturnStatus ReturnOperator(Environment& environment) {
    return Executable::ReturnStatus::kLeaveFunction;
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
    {"!", IntAssignmentOperator},
    {"f!", FloatAssignmentOperator},
    {"c!", CharAssignmentOperator},
    {"@", IntDereferenceOperator},
    {"f@", FloatDereferenceOperator},
    {"c@", CharDereferenceOperator},
    {"sinput", StringInputOperator},
    {"finput", FloatInputOperator},
    {"input", IntInputOperator},
    {"type", StringOutputOperator},
    {"emit", CharOutputOperator},
    {".", StackBackOutputOperator},
    {".s", AllStackOutputOperator},
    {"leave", BreakOperator},
    {"continue", ContinueOperator},
    {"return", ReturnOperator},
};