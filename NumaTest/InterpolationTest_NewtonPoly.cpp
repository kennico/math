#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;


class NewtonPolyTest :
    public testing::TestWithParam<std::vector<double>> {
public:
};

TEST_P(NewtonPolyTest, Quadratic) {

    auto d = GetParam();
    auto X = randf(d.front(), d.back());    
    auto expect = quadratic(X), result = 0.0;

    NewtonPoly::InputCont container;
    std::for_each(d.begin(), d.end(), [&](auto n) {
        container.emplace_back(n, quadratic(n));
    });

    NewtonPoly intrpl(container);
    result = intrpl.input(X);
    
    EXPECT_GE(0.00001, std::abs(result - expect)) 
        << "test input: " << X
        << " NewtonPoly - Result: " << result 
        << " Expect: " << expect;
}

INSTANTIATE_TEST_CASE_P(ReadFromFloatArray, NewtonPolyTest, ::testing::ValuesIn(data));






TEST(NewtonPolyTest, Sample) {
    auto X = 0.6;
    auto expect = 0.63665, result = 0.0;
    
    NewtonPoly::InputCont container = {
        {0.4,0.41075},{0.55,0.57815 },{0.65,0.69675 },{0.8,0.88811 },{0.9,1.02652 } 
    };
    
    NewtonPoly intrpl(container);
    result = intrpl.input(X);

    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << " NewtonPoly - Result: " << result << " Expected: " << expect;
}
