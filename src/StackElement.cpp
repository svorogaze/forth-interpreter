#include "StackElement.h"

StackElement::StackElement(std::variant<int64_t, double> other) {
    value = other;
}
