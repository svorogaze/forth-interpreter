#include "Executable.h"

Executable::ReturnStatus For::Execute(Environment &environment) {
    auto from = environment.GetStackBack().Convert<int64_t>();
    environment.PopStack();
    auto to = environment.GetStackBack().Convert<int64_t>();
    environment.PopStack();
    auto step = environment.GetStackBack().Convert<int64_t>();
    environment.PopStack();
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

