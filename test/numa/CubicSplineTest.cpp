#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class CubicSplineTest :
    public testing::TestWithParam<std::vector<double>> {
public:
};

TEST_P(CubicSplineTest, CubicGivenDerivAtTwoEnds) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back()), expect = cubicf(X);


    CubicSpline::InputCont cont;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        cont.emplace_back(n, cubicf(n));
    });

    CubicSpline intrpl(cont, cubicdy(cont.front().x), cubicdy(cont.back().x));
    auto result = intrpl.input(X);

    EXPECT_GE(0.00001, std::abs(result - expect))
                        << "test input: " << X
                        << " CubicSpline - Result: " << result
                        << " Expect: " << expect;
}


INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, CubicSplineTest,
                        ::testing::ValuesIn(getInput()->cbegin(), getInput()->cend()));

TEST(CubicSplineTest, Sample) {
    CubicSpline::InputCont cont = { {1,1},{2,3},{4,4},{5,2} };
    CubicSpline intrpl(cont);

    auto expect = 4.25, X = 3.0;
    auto result = intrpl.input(X);
    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << X
        << " CubicSpline - Result: " << result
        << " Expect: " << expect;
}

