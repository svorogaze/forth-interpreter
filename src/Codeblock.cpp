#include "Executable.h"

Executable::ReturnStatus Codeblock::Execute(Environment& environment) {
    for (const auto& executable : statements) {
        auto status = executable->Execute(environment);
        if (status != ReturnStatus::kSuccess) {
            return status;
        }
    }
    return ReturnStatus::kSuccess;
}
