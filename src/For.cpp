#include "Executable.h"

Executable::ReturnStatus For::Execute(Environment &environment) {
    auto from = environment.PopStack().Convert<int64_t>();
    auto to = environment.PopStack().Convert<int64_t>();
    auto step = environment.PopStack().Convert<int64_t>();
    auto old_ptr = environment.variables["I"];
    std::shared_ptr<int64_t> iptr(new int64_t);
    environment.variables["I"] = iptr.get();
    for (auto i = from; (step > 0 ? i < to : i > to); i += step) {
        *iptr = i;
        auto status = body->Execute(environment);
        if (status == ReturnStatus::kLeaveLoop) {
            break;
        }
        if (status == ReturnStatus::kLeaveFunction) {
            return status;
        }
    }
    environment.variables["I"] = old_ptr;
    return ReturnStatus::kSuccess;
}

