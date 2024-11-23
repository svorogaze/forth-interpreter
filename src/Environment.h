#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>
#include <map>
#include <string>
#include "StackElement.h"
#include "Executable.h"
class Environment {
public:
    std::vector<StackElement> stack;
    std::map<std::string, std::shared_ptr<Executable>> functions;
    std::map<std::string, void*> variables;
    std::vector<std::shared_ptr<Executable>> code;
};



#endif //ENVIRONMENT_H
