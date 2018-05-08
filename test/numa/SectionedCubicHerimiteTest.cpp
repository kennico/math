#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class SectionedCubicHermiteTest :
    public testing::TestWithParam<std::vector<double>> {
public:
};

TEST_P(SectionedCubicHermiteTest, Cubic) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back()), expect = cubicf(X);

    SectionedCubicHermite::InputCont cont;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        cont.emplace_back(n, cubicf(n), cubicdy(n));
    });

    SectionedCubicHermite intrpl(cont);
    auto result = intrpl.input(X);

    EXPECT_GE(0.00001, std::abs(result - expect))
        << "test input: " << X
        << " SectionedCubicHermite - Result: " << result
        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, SectionedCubicHermiteTest,
                        ::testing::ValuesIn(getInput()->cbegin(), getInput()->cend()));

