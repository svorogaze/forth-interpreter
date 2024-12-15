#ifndef STACKELEMENT_H
#define STACKELEMENT_H
#include <cstdint>
#include <variant>
#include <istream>
class StackElement {
public:
    std::variant<int64_t, double> value;
    template<typename T>
    StackElement(T other) {
        value = std::variant<int64_t, double>(other);
    }
    template<typename T>
    T Convert() {
        return std::visit([](auto a) {
            return *((T*)&a);
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
