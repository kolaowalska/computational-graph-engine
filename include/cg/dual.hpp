#pragma once
#include <cmath>
#include <iostream>

namespace cg {

    template<typename T>
    struct Dual {
        T value; // value
        T d; // derivative

        Dual(T v = 0, T d = 0) : value(v), d(d) {}

        Dual operator+(const Dual& x) const {
            return {value + x.value, d + x.d};
        }

        Dual operator-(const Dual& x) const {
            return {value - x.value, d - x.d};
        }

        Dual operator*(const Dual& x) const {
            return {value * x.value, value * x.d + d * x.value};
        }

        Dual operator/(const Dual& x) const {
            return {value / x.value, (d * x.value - value * x.d) / (x.value * x.value)};
        }

        Dual operator-() const {
            return {-value, -d};
        }

        auto operator<=>(const Dual& x) const = default;
    };

    template<typename T> Dual<T> operator+(T a, const Dual<T>& x) { return {a + x.value, x.d}; }
    template<typename T> Dual<T> operator+(const Dual<T>& x, T a) { return {x.value + a, x.d}; }
    template<typename T> Dual<T> operator-(T a, const Dual<T>& x) { return {a - x.value, -x.d}; }
    template<typename T> Dual<T> operator-(const Dual<T>& x, T a) { return {x.value - a, x.d}; }
    template<typename T> Dual<T> operator*(T a, const Dual<T>& x) { return {a * x.value, a * x.d}; }
    template<typename T> Dual<T> operator*(const Dual<T>& x, T a) { return {x.value * a, x.d * a}; }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Dual<T>& x) {
        return os << "{value: " << x.value << ", d: " << x.d << "}";
    }

    template<typename T> Dual<T> sin(const Dual<T>& x) {
        return {std::sin(x.value), std::cos(x.value) * x.d};
    }

    template<typename T> Dual<T> cos(const Dual<T>& x) {
        return {std::cos(x.value), -std::sin(x.value) * x.d};
    }

    template<typename T> Dual<T> exp(const Dual<T>& x) {
        T e = std::exp(x.value);
        return {e, e * x.d};
    }

    // TODO: add log(x) ?

}