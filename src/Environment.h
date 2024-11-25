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
    std::vector<std::shared_ptr<Executable>> code;
    void PopStack();
    StackElement GetStackBack();
    void PushOnStack(StackElement s);
private:
    std::vector<StackElement> stack;
};



#endif //ENVIRONMENT_H
