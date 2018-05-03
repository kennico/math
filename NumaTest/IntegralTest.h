#pragma once
#include "test.h"


namespace knylaw {
namespace math {
namespace test {


inline double quadratic(double x) {
    return x * x;
}

inline double quadraticAntiDeriv(double x) {
    return quadratic(x) * x / 3.0;
}

inline double linear(double x) {
    return x * 3;
}

inline double linearAntiDeriv(double x) {
    return linear(x) * x / 2.0;
}


}
}
}