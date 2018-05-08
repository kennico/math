#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class NewtonPolyTest :
        public testing::TestWithParam<InputXCoords> {
public:
};

TEST_P(NewtonPolyTest, Cubic) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back()), expect = cubicf(X);

    NewtonPoly::InputCont cont;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        cont.emplace_back(n, cubicf(n));
    });

    NewtonPoly intrpl(cont);
    auto result = intrpl.input(X);

    EXPECT_GE(0.00001, std::abs(result - expect))
                        << "test input: " << X
                        << " NewtonPoly - Result: " << result
                        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, NewtonPolyTest,
                        ::testing::ValuesIn(getInput()->cbegin(), getInput()->cend()));


TEST(NewtonPolyTest, Sample) {
    auto X = 0.6, expect = 0.63665;

    NewtonPoly::InputCont cont = {
            {0.4,0.41075},{0.55,0.57815 },{0.65,0.69675 },{0.8,0.88811 },{0.9,1.02652 }
    };

    NewtonPoly intrpl(cont);
    auto result = intrpl.input(X);

    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << " NewtonPoly - Result: " << result << " Expected: " << expect;
}
