#include "Executable.h"

Executable::ReturnStatus If::Execute(Environment& environment) {
    auto bool_flag = environment.PopStack();
    if (bool_flag.Convert<bool>()) {
        return if_part->Execute(environment);
    }
    if (else_part) {
        return else_part->Execute(environment);
    }
    return ReturnStatus::kSuccess;
}

