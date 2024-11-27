#ifndef EXECUTABLE_H
#define EXECUTABLE_H
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "Environment.h"


class Executable {
public:
    enum class ReturnStatus {
        kSuccess,
        kLeaveLoop,
        kLeaveFunction,
    };
    virtual ReturnStatus Execute(Environment& environment) = 0;
    virtual ~Executable() = default;
};

class VariableCreation final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    std::string name;
    int64_t size;
    std::string type;
};

class Codeblock final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    std::vector<std::shared_ptr<Executable>> statements;
};

class While final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    std::shared_ptr<Executable> condition;
    std::shared_ptr<Executable> body;
};

class For final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    std::shared_ptr<Executable> body;
};

class If final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    std::shared_ptr<Executable> if_part;
    std::shared_ptr<Executable> else_part;
};

class Switch final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    std::map<int64_t, std::shared_ptr<Executable>> cases;
};

class Operator final : public Executable {
public:
    ReturnStatus Execute(Environment& environment) override;
    explicit Operator(std::string text);
    std::string text;
    static std::map<std::string, std::function<ReturnStatus (Environment&)>> operators_pointers;
private:
    ReturnStatus FunctionCall(Environment& environment);
    ReturnStatus VariableUse(Environment& environment);
    ReturnStatus Literal(Environment& environment);
};

#endif //EXECUTABLE_H
