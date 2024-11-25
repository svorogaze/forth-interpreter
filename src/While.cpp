#include "Executable.h"

Executable::ReturnStatus While::Execute(Environment& environment) {
    while (true) {
        auto status = condition->Execute(environment);
        if (status != ReturnStatus::kSuccess) {
            if (status == ReturnStatus::kLeaveLoop) {
                break;
            }
            return status;
        }
        auto elem = environment.PopStack();
        if (!elem.Convert<bool>()) {
            break;
        }
        status = body->Execute(environment);
        if (status != ReturnStatus::kSuccess) {
            if (status == ReturnStatus::kLeaveLoop) {
                break;
            }
            return status;
        }
    }
    return ReturnStatus::kSuccess;
}
