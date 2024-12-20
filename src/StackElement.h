/**
 * @file StackElement.h
 * @brief Defines the StackElement class for handling stack operations and arithmetic.
 */

#ifndef STACKELEMENT_H
#define STACKELEMENT_H

#include <cstdint>
#include <variant>
#include <istream>
#include <type_traits>

/**
 * @brief Concept to define types that can be represented as rational numbers.
 */
template<typename T>
concept Rational = std::integral<T> || std::is_same_v<double, T> || std::is_same_v<float, T>;

/**
 * @class StackElement
 * @brief Represents an element on the stack that supports various operations.
 */
class StackElement {
public:
    /**
     * @brief Holds the value of the stack element, which can be an integer or a double.
     */
    std::variant<int64_t, double> value;

    /**
     * @brief Constructs a StackElement from a given value.
     * @tparam T The type of the value, which must be compatible with the stack element.
     * @param other The value to initialize the StackElement.
     */
    template<typename T>
    StackElement(T other) {
        value = std::variant<int64_t, double>(other);
    }

    /**
     * @brief Converts the stack element to a specified type.
     * @tparam T The type to convert to.
     * @return The converted value.
     */
    template<typename T>
    T Convert() {
        return std::visit([](auto a) {
            return *((T*)&a);
        }, value);
    }

    /**
     * @brief Converts the stack element to a specified rational type.
     * @tparam T The rational type to convert to.
     * @return The converted value.
     */
    template<Rational T>
    T Convert() {
        return std::visit([](auto a) {
            return (T)a;
        }, value);
    }

    // Arithmetic and bitwise operators

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

/**
 * @brief Outputs the value of a StackElement to an output stream.
 * @param out The output stream.
 * @param val The StackElement to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& out, const StackElement& val);

#endif // STACKELEMENT_H
