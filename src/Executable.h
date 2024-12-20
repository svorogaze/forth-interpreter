/**
 * @file Executable.h
 * @brief Defines the Executable interface and its derived classes for program execution.
 */

#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "Environment.h"

/**
 * @class Executable
 * @brief Abstract base class for executable entities within the environment.
 */
class Executable {
public:
    /**
     * @brief Enum representing the return status of an execution.
     */
    enum class ReturnStatus {
        kSuccess,        ///< Execution completed successfully.
        kLeaveLoop,      ///< Leave the current loop.
        kLeaveFunction,  ///< Leave the current function.
        kContinueLoop    ///< Continue to the next iteration of the loop.
    };

    /**
     * @brief Executes the entity within the given environment.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    virtual ReturnStatus Execute(Environment& environment) = 0;

    /**
     * @brief Virtual destructor for the Executable class.
     */
    virtual ~Executable() = default;
};

/**
 * @class VariableCreation
 * @brief Represents the creation of a variable in the environment.
 */
class VariableCreation final : public Executable {
public:
    /**
     * @brief Executes the variable creation operation.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::string name; ///< The name of the variable to be created.
    int64_t size;    ///< The size of the variable.
    std::string type; ///< The type of the variable.
};

/**
 * @class Codeblock
 * @brief Represents a block of executable statements.
 */
class Codeblock final : public Executable {
public:
    /**
     * @brief Executes all statements within the code block.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::vector<std::shared_ptr<Executable>> statements; ///< The statements to execute.
};

/**
 * @class While
 * @brief Represents a while loop structure.
 */
class While final : public Executable {
public:
    /**
     * @brief Executes the while loop.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::shared_ptr<Executable> condition; ///< The condition for the while loop.
    std::shared_ptr<Executable> body;      ///< The body of the while loop.
};

/**
 * @class For
 * @brief Represents a for loop structure.
 */
class For final : public Executable {
public:
    /**
     * @brief Executes the for loop.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::shared_ptr<Executable> body; ///< The body of the for loop.
};

/**
 * @class If
 * @brief Represents an if-else control structure.
 */
class If final : public Executable {
public:
    /**
     * @brief Executes the if-else structure.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::shared_ptr<Executable> if_part;   ///< The statements to execute if the condition is true.
    std::shared_ptr<Executable> else_part; ///< The statements to execute if the condition is false.
};

/**
 * @class Switch
 * @brief Represents a switch-case control structure.
 */
class Switch final : public Executable {
public:
    /**
     * @brief Executes the switch-case structure.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::map<int64_t, std::shared_ptr<Executable>> cases; ///< The cases for the switch statement.
};

/**
 * @class Operator
 * @brief Represents an operator or operation in the environment.
 */
class Operator final : public Executable {
public:
    /**
     * @brief Constructs an Operator with the given text.
     * @param text The text representing the operator.
     */
    explicit Operator(std::string text);

    /**
     * @brief Executes the operator.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Execute(Environment& environment) override;

    std::string text; ///< The text representing the operator.

    /**
     * @brief A map of operator names to their corresponding functions.
     */
    static std::map<std::string, std::function<ReturnStatus (Environment&)>> operators_pointers;

private:
    /**
     * @brief Executes a function call operation.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus FunctionCall(Environment& environment);

    /**
     * @brief Handles variable use during execution.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus VariableUse(Environment& environment);

    /**
     * @brief Processes a literal value during execution.
     * @param environment The execution environment.
     * @return The return status of the execution.
     */
    ReturnStatus Literal(Environment& environment);
};

#endif //EXECUTABLE_H
