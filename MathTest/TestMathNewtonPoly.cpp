#include "pch.h"
#include <fstream>

#include "utils.h"
#include "numa.h"
#pragma comment(lib, "math.lib")

using Float = knylaw::math::Float;
using Point = knylaw::math::Point;

TEST(TestMathNewtonPoly, Sample) {
    std::vector<Point> data = {
         {0.8, 0.88811},{ 0.4, 0.41075 } ,{0.9, 1.02652},{ 0.65, 0.69675 },{ 0.55, 0.57815 }
    };

    knylaw::math::NewtonPolynormial poly;
    poly.accept(data.begin(), data.end());
    poly.computeDiffs();

    auto r = poly.input(tx);
    auto e = f(tx);
    auto diff = std::abs(r - e);

    EXPECT_GE(0.00000001, diff) << "Result: " << r << " Expect: " << e;
}

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}