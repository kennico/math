#include "pch.h"
#include <fstream>

#include "utils.h"
#include "numa.h"
#pragma comment(lib, "math.lib")


using Point = knylaw::math::Point;
using knylaw::math::interpolation;
using knylaw::math::NewtonPoly;



TEST(TestMathNewtonPoly, Sample) {
    std::vector<Point> data = {
         {0.8, 0.88811},{ 0.4, 0.41075 } ,{0.9, 1.02652},{ 0.65, 0.69675 },{ 0.55, 0.57815 }
    };

    auto poly = interpolation<NewtonPoly>(data.begin(), data.end());
    auto r = poly.input(0.6);
    auto e = 0.63665;
    auto diff = std::abs(r - e);

    EXPECT_GE(0.00001, diff) << "Result: " << r << " Expect: " << e;
}

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}