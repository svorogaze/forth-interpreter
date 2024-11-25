#ifndef STACKELEMENT_H
#define STACKELEMENT_H
#include <cstdint>
#include <variant>

class StackElement {
public:
    std::variant<int64_t, double> value;
    StackElement(std::variant<int64_t, double>);
    template<typename T>
    T Convert();
};



#endif //STACKELEMENT_H
