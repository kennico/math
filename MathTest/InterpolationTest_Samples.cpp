#include "InterpolationTest.h"

using namespace knylaw::math;
using namespace knylaw::math::test;

const static Float XX[] = { 0.4,0.55,0.65,0.8,0.9 };
const static size_t COUNT = sizeof(XX) / sizeof(Float);
const static Float X = 0.6;

TEST(InterpolationTest, SampleFunction0) {
	Float expect = 0.63665, result;
    const static Float YY[] = { 0.41075,0.57815,0.69675,0.88811,1.02652 };

	result = interpolationp<NewtonPoly>(XX, XX+COUNT, YY).input(X);
	EXPECT_GE(0.0001, std::abs(result - expect)) \
		<< "Result: " << result << " Expected: " << expect;

    result = interpolationp<LagrangePoly>(XX, XX + COUNT, YY).input(X);
    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << "Result: " << result << " Expected: " << expect;
}

TEST(InterpolationTest, SampleFunction1) {
    Float expect = quadratic(X), result;

    result = interpolationf<NewtonPoly>(XX, XX + COUNT, quadratic).input(X);
	EXPECT_GE(0.0001, std::abs(result - expect)) \
		<< "Result: " << result << " Expected: " << expect;

    result = interpolationf<LagrangePoly>(XX, XX + COUNT, quadratic).input(X);
    EXPECT_GE(0.0001, std::abs(result - expect)) \
        << "Result: " << result << " Expected: " << expect;
}

