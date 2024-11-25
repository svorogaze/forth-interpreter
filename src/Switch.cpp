#include "Executable.h"

Executable::ReturnStatus Switch::Execute(Environment& environment) {
    auto selector = environment.GetStackBack().Convert<int64_t>();
    environment.PopStack();
    if (cases.find(selector) != cases.end()) {
        return cases[selector]->Execute(environment);
    }
    return ReturnStatus::kSuccess;
}

