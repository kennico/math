#pragma once
#include "test.h"

namespace knylaw {
namespace math {
namespace test {


inline double quadratic(double n) {
    return n * n;
};

inline double quadraticdy(double n) {
    return 2 * n;
}

inline double randf(double from, double to) {
    // https://stackoverflow.com/questions/686353/c-random-float-number-generation
    return from + (rand() / (RAND_MAX / (to - from)));
}


extern const std::list<std::vector<double>> data;


}
}
}