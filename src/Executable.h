#ifndef EXECUTABLE_H
#define EXECUTABLE_H
#include <memory>

#include "Environment.h"
#include <vector>



class Executable {
public:
    enum class ReturnStatus {
        kSuccess,
        kLeaveLoop,
        kLeaveFunction,
    };
    virtual ReturnStatus Execute(Environment& environment);
    virtual ~Executable();
};

class Operator final : public Executable {
public:
    ReturnStatus Execute(Environment &environment) override;
    explicit Operator(std::string text);
    std::string text;
};

class VariableCreation final : public Executable {
public:
    ReturnStatus Execute(Environment &environment) override;
    std::string name;
    int64_t size;
    std::string type;
};

class Codeblock final : public Executable {
public:
    ReturnStatus Execute(Environment &environment) override;
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
    ReturnStatus Execute(Environment &environment) override;
    std::shared_ptr<Executable> if_part;
    std::shared_ptr<Executable> else_part;
};

class Switch final : public Executable {
public:
    ReturnStatus Execute(Environment &environment) override;
    std::map<int64_t, std::shared_ptr<Executable>> cases;
};

#endif //EXECUTABLE_H
