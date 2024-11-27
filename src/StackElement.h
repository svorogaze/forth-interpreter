#ifndef STACKELEMENT_H
#define STACKELEMENT_H
#include <cstdint>
#include <variant>
#include <istream>
class StackElement {
public:
    std::variant<int64_t, double> value;
    StackElement(const std::variant<int64_t, double>&);
    template<typename T>
    T Convert() {
        return std::visit([](auto a) {
            return static_cast<T>(a);
        },value);
    }
    StackElement operator+(const StackElement& other);
    StackElement operator-(const StackElement& other);
    StackElement operator-();
    StackElement operator*(const StackElement& other);
    StackElement operator/(const StackElement& other);
    StackElement operator%(const StackElement& other);
    StackElement operator~();
    StackElement operator!();
    StackElement operator&(const StackElement& other);
    StackElement operator^(const StackElement& other);
    StackElement operator|(const StackElement& other);
    StackElement operator<(const StackElement& other);
    StackElement operator<=(const StackElement& other);
    StackElement operator>(const StackElement& other);
    StackElement operator>=(const StackElement& other);
    StackElement operator==(const StackElement& other);
};

std::ostream& operator<<(std::ostream& out, const StackElement& val);


#endif //STACKELEMENT_H
