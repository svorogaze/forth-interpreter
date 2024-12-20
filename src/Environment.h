/**
 * @file Environment.h
 * @brief Defines the stack operations and environment structure for the Environment class.
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "StackElement.h"
class Executable;

/**
 * @class Environment
 * @brief Represents an execution environment with stack, variables, and functions.
 */
class Environment {
public:
    /**
     * @brief A map of function names to their corresponding executable objects.
     */
    std::map<std::string, std::shared_ptr<Executable>> functions;

    /**
     * @brief A map of variable names to their corresponding values.
     *
     * The values are stored as void pointers to allow flexibility in data types.
     */
    std::map<std::string, void*> variables;

    /**
     * @brief A shared pointer to the main executable code for the environment.
     */
    std::shared_ptr<Executable> code;

    /**
     * @brief Removes and returns the top element from the stack.
     *
     * This method checks if the stack is empty before attempting to remove an element.
     * If the stack is empty, it throws a `std::runtime_error`.
     *
     * @return The top element of the stack.
     * @throws std::runtime_error If the stack is empty.
     */
    StackElement PopStack();

    /**
     * @brief Pushes a new element onto the stack.
     *
     * This method adds the provided element to the top of the stack.
     *
     * @param s The element to be pushed onto the stack.
     */
    void PushOnStack(StackElement s);

    /**
     * @brief The stack used to manage execution state and data.
     */
    std::vector<StackElement> stack;

private:
};

#endif //ENVIRONMENT_H
