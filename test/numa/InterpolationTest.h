#pragma once
#include <gtest/gtest.h>
#include <list>
#include <vector>

#include "numa/intrpl.h"

namespace knylaw {
namespace math {
namespace test {


//    inline double quadratic(double n) {
//        return n * n;
//    };
//
//    inline double quadraticdy(double n) {
//        return 2 * n;
//    }

    inline double cubicf(double n) {
        return 2.015 * n * n * n + 0.30541 * n * n + 85 * n + 2;
    }

    inline double cubicdy(double n) {
        return 2.015 * 3 * n * n + 0.30541 * 2 * n + 85;
    }

    inline double randf(double from, double to) {
        // https://stackoverflow.com/questions/686353/c-random-float-number-generation
        return from + (rand() / (RAND_MAX / (to - from)));
    }

    using InputXCoords = std::vector<double>;

    const std::list<InputXCoords>* getInput();


}
}
}