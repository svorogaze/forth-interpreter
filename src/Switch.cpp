#include "Executable.h"

Executable::ReturnStatus Switch::Execute(Environment& environment) {
    auto selector = environment.PopStack().Convert<int64_t>();
    if (cases.find(selector) != cases.end()) {
        return cases[selector]->Execute(environment);
    }
    return ReturnStatus::kSuccess;
}

