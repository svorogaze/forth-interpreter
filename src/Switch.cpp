#include "Executable.h"

Executable::ReturnStatus Switch::Execute(Environment& environment) {
    auto selector = environment.PopStack().Convert<int64_t>();
    if (cases.contains(selector)) {
        return cases[selector]->Execute(environment);
    }
    return ReturnStatus::kSuccess;
}

