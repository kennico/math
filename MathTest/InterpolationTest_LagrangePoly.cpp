#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class LagrangePolyTest :
    public testing::TestWithParam<std::vector<double>> {
public:
};

TEST_P(LagrangePolyTest, Quadratic) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back());
    auto expect = quadratic(X), result = 0.0;

    Helper<LagrangePoly>::Container container;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        container.emplace_back(n, quadratic(n));
    });

    auto intrpl = Helper<LagrangePoly>().from(container);
    result = intrpl.input(X);

    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << X
        << " LagrangePoly - Result: " << result
        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, LagrangePolyTest, ::testing::ValuesIn(data));






TEST(LagrangePolyTest, Sample) {

    const static double XX[] = { 0.4,0.55,0.65,0.8,0.9 };
    const static size_t COUNT = sizeof(XX) / sizeof(double);
    const static double X = 0.6;
    const static double YY[] = { 0.41075,0.57815,0.69675,0.88811,1.02652 };
    double expect = 0.63665, result;

    Helper<LagrangePoly>::Container container;
    knylaw::for_each(XX, XX + COUNT, YY, [&](auto x, auto y) {
        container.emplace_back(x, y);
    });


    auto intrpl = Helper<LagrangePoly>().from(container);
    result = intrpl.input(X);

    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << " LagrangePoly - Result: " << result << " Expected: " << expect;
}
