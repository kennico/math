#pragma once
#include <algorithm>

namespace knylaw {
namespace math {

    template<typename Func, typename Real=double>
    Real trapezoidal(Func func, Real a, Real b, size_t times) {
        Real h = (b - a) / times;
        Real result = 0.0;
        for (size_t k = 1; k < times; k++) {
            result += func(a + k * h);
        }
        return h * ((func(a) + func(b)) / 2.0 + result);
    }

    namespace impl {

        template<typename Func, typename Real=double>
        Real trapezoidalDouble(Func f, size_t n, Real tn, Real a, Real b) {
            Real sum = 0.0, h = (b-a) / n;
            for (size_t k = 1; k <= n; k++) {
                sum += f(a + (k*2.0 - 1)*h / 2.0);
            }
            return (tn + h*sum) / 2.0;
        }

    }

    // in case of endless loop
    template<typename Func, typename Real=double>
    Real trapezoidal(Func func, Real a, Real b, Real error, size_t maxrefinements=32) {
        error = std::abs(error) * 3;
        Real result = trapezoidal(func, a, b, 1);
        size_t n = 1, times = 0;
        
        while (times++ < maxrefinements) {
            Real last = result;
            result = impl::trapezoidalDouble(func, n, result, a, b);
            n = n * 2;
            if (std::abs(last - result) < error) {
                break;
            }
        }
        return result;
    }

    template<typename Func, typename Real=double>
    Real romberg(Func func, Real a, Real b, Real error, size_t maxrefinements=32) {

        Real* prev = new Real[maxrefinements + 1];
        Real* next = new Real[maxrefinements + 1];
        
        error = std::abs(error);
        Real result = prev[0] = trapezoidal(func, a, b, 1);

        for (size_t times = 1, n = 1; times <= maxrefinements ; times++, n *= 2) {
            next[0] = impl::trapezoidalDouble(func, n, prev[0], a, b);

            size_t i = 1;
            for (; i <= times; i++) {
                next[i] = (4 * next[i-1] - prev[i-1]) / 3.0;
            }
            result = next[i-1];

            if (std::abs(result - prev[i-2]) < error) {
                break;
            } 

            std::swap(prev, next);
        }

        delete[] prev;
        delete[] next;

        return result;
    }

}
}