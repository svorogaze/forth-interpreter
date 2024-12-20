#include <string>
#include "Executable.h"

Executable::ReturnStatus VariableCreation::Execute(Environment& environment) {
    if (environment.variables.contains(name)) {
        std::string s = "Variable " + name + " is already defined";
        throw std::runtime_error(s);
    }
    size_t byte_size = size;
    if (type == "cells" || type == "floats") {
        byte_size *= 8;
    }
    void* allocated_memory = malloc(byte_size);
    environment.variables[name] = allocated_memory;
    return ReturnStatus::kSuccess;
}
