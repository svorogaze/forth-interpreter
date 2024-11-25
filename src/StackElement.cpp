#include "StackElement.h"

StackElement::StackElement(std::variant<int64_t, double> other) {
    value = other;
}

template<typename T>
T StackElement::Convert() {
    return std::visit([](auto a) {
        return static_cast<T>(a);
    },value);
}
