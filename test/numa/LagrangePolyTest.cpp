#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class LagrangePolyTest :
        public testing::TestWithParam<InputXCoords> {
public:
};

TEST_P(LagrangePolyTest, Cubic) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back()), expect = cubicf(X);

    LagrangePoly::InputCont cont;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        cont.emplace_back(n, cubicf(n));
    });

    LagrangePoly intrpl(cont);
    auto result = intrpl.input(X);

    EXPECT_GE(0.00001, std::abs(result - expect))
                        << "test input: " << X
                        << " LagrangePoly - Result: " << result
                        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, LagrangePolyTest,
                        ::testing::ValuesIn(getInput()->cbegin(), getInput()->cend()));


TEST(LagrangePolyTest, Sample) {
    auto X = 0.6, expect = 0.63665;

    LagrangePoly::InputCont cont = {
            {0.4,0.41075},{0.55,0.57815 },{0.65,0.69675 },{0.8,0.88811 },{0.9,1.02652 }
    };

    LagrangePoly intrpl(cont);
    auto result = intrpl.input(X);

    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << " LagrangePoly - Result: " << result << " Expected: " << expect;
}
