#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;




class LagrangePolyTest :
    public testing::TestWithParam<std::vector<Float>> {
public:
};

TEST_P(LagrangePolyTest, Quadratic) {

    auto d = GetParam();
    auto minmax = std::minmax_element(d.begin(), d.end());
    auto testx = randf(*minmax.first, *minmax.second);
    Float expect = quadratic(testx), result;

    result = interpolationf<LagrangePoly>(d.begin(), d.end(), quadratic).input(testx);
    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << testx
        << " LagrangePoly - Result: " << result
        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, LagrangePolyTest, ::testing::ValuesIn(data));




const static Float XX[] = { 0.4,0.55,0.65,0.8,0.9 };
const static size_t COUNT = sizeof(XX) / sizeof(Float);
const static Float X = 0.6;

TEST(LagrangePolyTest, Sample) {
    Float expect = 0.63665, result;
    const static Float YY[] = { 0.41075,0.57815,0.69675,0.88811,1.02652 };

    result = interpolationp<LagrangePoly>(XX, XX + COUNT, YY).input(X);
    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << "Result: " << result << " Expected: " << expect;
}
