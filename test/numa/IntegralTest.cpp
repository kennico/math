#include "IntegralTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;

TEST(Trapezoidal, Adaptive) {
    
    auto e = 0.0000001;
    auto a = 66.0, b = 99.0;

    auto result = trapezoidal(quadratic, a, b, e);
    auto expect = quadraticAntiDeriv(b) - quadraticAntiDeriv(a);

    EXPECT_GE(e, std::abs(result - expect)) << "Result: " << result << " Expect: " << expect;

}

TEST(Trapezoidal, Composite) {
    auto e = 0.0000001;
    auto a = 66.0, b = 99.0;
    size_t times = 32;

    auto result = trapezoidal(linear, a, b, times);
    auto expect = linearAntiDeriv(b) - linearAntiDeriv(a);


    EXPECT_GE(e, std::abs(result - expect)) << "Result: " << result << " Expect: " << expect;

}

TEST(Romberg, Quadratic) {
    auto e = 0.0000001;
    auto a = 66.0, b = 99.0;

    auto result = romberg(quadratic, a, b, e, 27);
    auto expect = quadraticAntiDeriv(b) - quadraticAntiDeriv(a);

    EXPECT_GE(e, std::abs(result - expect)) << "Result: " << result << " Expect: " << expect;

}