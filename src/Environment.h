#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "StackElement.h"
class Executable;
class Environment {
public:
    std::map<std::string, std::shared_ptr<Executable>> functions;
    std::map<std::string, void*> variables;
    std::shared_ptr<Executable> code;
    StackElement PopStack();
    void PushOnStack(StackElement s);
    std::vector<StackElement> stack;
private:
};



#endif //ENVIRONMENT_H
