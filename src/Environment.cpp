#include "Environment.h"
#include "stdexcept"

void Environment::PopStack() {
    if (stack.empty()) {
        throw std::runtime_error("Stack is empty when popping it");
    }
    stack.pop_back();
}

StackElement Environment::GetStackBack() {
    if (stack.empty()) {
        throw std::runtime_error("Zero elements on stack when trying to get it's back");
    }
    return stack.back();
}

void Environment::PushOnStack(StackElement s) {
    stack.push_back(s);
}


