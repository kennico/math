#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;



class CubicSplineTest :
    public testing::TestWithParam<std::vector<Float>> {
public:
};

TEST_P(CubicSplineTest, Quadratic) {

    auto d = GetParam();
    auto minmax = std::minmax_element(d.begin(), d.end());
    auto testx = randf(*minmax.first, *minmax.second);
    Float expect = quadratic(testx), result;

    result = interpolationf<CubicSpline>(d.begin(), d.end(), quadratic).input(testx);
    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << testx
        << " CubicSpline - Result: " << result
        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, CubicSplineTest, ::testing::ValuesIn(data));





const static size_t COUNT = 4;
const static Float XX[COUNT] = { 1,2,4,5 }, YY[COUNT] = { 1,3,4,2 };


TEST(CubicSplineTest, Sample) {
    Float expect = 4.25, result;

    result = interpolationp<CubicSpline>(XX, XX + COUNT, YY).input(3);

    EXPECT_GE(0.1, std::abs(expect - result)) << "Result: " << result << "Expected: " << expect;
}