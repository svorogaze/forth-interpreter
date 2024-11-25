#include "Executable.h"

Executable::ReturnStatus For::Execute(Environment &environment) {
    auto from = environment.PopStack().Convert<int64_t>();
    auto to = environment.PopStack().Convert<int64_t>();
    auto step = environment.PopStack().Convert<int64_t>();
    for (auto i = from; (step > 0 ? i < to : i > to); i += step) {
        auto status = body->Execute(environment);
        if (status == ReturnStatus::kLeaveLoop) {
            break;
        }
        if (status == ReturnStatus::kLeaveFunction) {
            return status;
        }
    }
    return ReturnStatus::kSuccess;
}

