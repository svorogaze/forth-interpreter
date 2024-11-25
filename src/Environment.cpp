#include "Environment.h"
#include "stdexcept"

StackElement Environment::PopStack() {
    if (stack.empty()) {
        throw std::runtime_error("Zero elements on stack when popping it");
    }
    auto res = stack.back();
    stack.pop_back();
    return res;
}

void Environment::PushOnStack(StackElement s) {
    stack.push_back(s);
}


