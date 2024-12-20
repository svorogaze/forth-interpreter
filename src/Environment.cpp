#include "Environment.h"
#include "stdexcept"
/**
 * @class Environment
 * @brief Manages a stack of elements within the environment.
 */

/**
 * @brief Removes and returns the top element from the stack.
 *
 * This method checks if the stack is empty before attempting to remove an element.
 * If the stack is empty, it throws a `std::runtime_error`.
 *
 * @return The top element of the stack.
 * @throws std::runtime_error If the stack is empty.
 */
StackElement Environment::PopStack() {
    if (stack.empty()) {
        throw std::runtime_error("Zero elements on stack when popping it");
    }
    auto res = stack.back();
    stack.pop_back();
    return res;
}
/**
 * @brief Pushes a new element onto the stack.
 *
 * This method adds the provided element to the top of the stack.
 *
 * @param s The element to be pushed onto the stack.
 */
void Environment::PushOnStack(StackElement s) {
    stack.push_back(s);
}
