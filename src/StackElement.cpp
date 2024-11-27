#include "StackElement.h"

StackElement::StackElement(const std::variant<int64_t, double>& other) {
    value = other;
}

StackElement StackElement::operator+(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(a + b);
    }, value, other.value);
}

StackElement StackElement::operator-(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(a - b);
    }, value, other.value);
}

StackElement StackElement::operator-() {
    return std::visit([](auto a) {
       return StackElement(-a);
    }, value);
}

StackElement StackElement::operator*(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(a * b);
    }, value, other.value);
}

StackElement StackElement::operator/(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(a / b);
    }, value, other.value);
}

StackElement StackElement::operator%(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(static_cast<int64_t>(a) % static_cast<int64_t>(b));
    }, value, other.value);
}

StackElement StackElement::operator~() {
    return std::visit([](auto a) {
       return StackElement(~static_cast<int64_t>(a));
    }, value);
}

StackElement StackElement::operator!() {
    return std::visit([](auto a) {
       return StackElement(!a);
    }, value);
}

StackElement StackElement::operator&(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(static_cast<int64_t>(a) & static_cast<int64_t>(b));
    }, value, other.value);
}

StackElement StackElement::operator|(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(static_cast<int64_t>(a) | static_cast<int64_t>(b));
    }, value, other.value);
}

StackElement StackElement::operator^(const StackElement& other) {
    return std::visit([](auto a, auto b) {
       return StackElement(static_cast<int64_t>(a) ^ static_cast<int64_t>(b));
    }, value, other.value);
}

StackElement StackElement::operator<(const StackElement& other) {
    auto result = std::visit([](auto a, auto b) {
       return a < b;
    }, value, other.value);
    return StackElement(result);
}

StackElement StackElement::operator<=(const StackElement& other) {
    auto result = std::visit([](auto a, auto b) {
       return a <= b;
    }, value, other.value);
    return StackElement(result);
}

StackElement StackElement::operator>(const StackElement& other) {
    auto result = std::visit([](auto a, auto b) {
       return a > b;
    }, value, other.value);
    return StackElement(result);
}

StackElement StackElement::operator>=(const StackElement& other) {
    auto result = std::visit([](auto a, auto b) {
       return a >= b;
    }, value, other.value);
    return StackElement(result);
}

StackElement StackElement::operator==(const StackElement& other) {
    auto result = std::visit([](auto a, auto b) {
       return a == b;
    }, value, other.value);
    return StackElement(result);
}

std::ostream& operator<<(std::ostream& out, const StackElement& val) {
    std::visit([&out](auto a) {
       out << a;
    }, val.value);
    return out;
}
