#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class CubicSplineTest :
    public testing::TestWithParam<std::vector<double>> {
public:
};

TEST_P(CubicSplineTest, QuadraticGivenDeriativeAtTwoEnds) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back());
    auto expect = quadratic(X), result = 0.0;
    

    CubicSpline::InputCont cont;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        cont.emplace_back(n, quadratic(n));
    });

    CubicSpline intrpl(cont, quadraticdy(cont.front().x), quadraticdy(cont.back().x));
    result = intrpl.input(X);

    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << X
        << " CubicSpline - Result: " << result
        << " Expect: " << expect;
}

//TEST_P(CubicSplineTest, QuadraticNoDerivativeTwoEnds) {
//
//    auto d = GetParam();
//    auto X = randf(d.front(), d.back());
//    auto expect = quadratic(X), result = 0.0;
//
//
//    CubicSpline::InputCont cont;
//    std::for_each(d.begin(), d.end(), [&](auto n) {
//        cont.emplace_back(n, quadratic(n));
//    });
//
//    CubicSpline intrpl(cont);
//    result = intrpl.input(X);
//
//    EXPECT_GE(0.00001, std::abs(result - expect))
//        << "test input: " << X
//        << " CubicSpline - Result: " << result
//        << " Expect: " << expect;
//}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, CubicSplineTest, ::testing::ValuesIn(data));

TEST(CubicSplineTest, Sample) {
    CubicSpline::InputCont cont = { {1,1},{2,3},{4,4},{5,2} };
    CubicSpline intrpl(cont);

    auto expect = 4.25, result = 0.0, X = 3.0;
    result = intrpl.input(X);
    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << X
        << " CubicSpline - Result: " << result
        << " Expect: " << expect;
}

