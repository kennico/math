#include "pch.h"
#include <fstream>

#include "utils.h"
#include "numa.h"
#pragma comment(lib, "math.lib")




using Float = knylaw::math::Float;
using FloatVector = std::vector<Float>;
using knylaw::math::interpolation;
using knylaw::math::NewtonPoly;



std::list<FloatVector> inputs;

class TestMathNewtonPoly :
    public testing::TestWithParam<FloatVector> {
public:
   
};

double randf(double from, double to) {
    // https://stackoverflow.com/questions/686353/c-random-float-number-generation
    return from + (rand() / (RAND_MAX / (to - from)));
}

TEST_P(TestMathNewtonPoly, SquareFunction) {

    auto square = [](Float n) {
        return n * n;
    };

    auto data = GetParam();
    
    auto poly = interpolation<NewtonPoly>(data.begin(), data.end(), square);

    auto minmax = std::minmax_element(data.begin(), data.end());
    auto tx = randf(*minmax.first, *minmax.second);

    auto r = poly.input(tx);
    auto e = square(tx);
    auto diff = std::abs(r-e);

    EXPECT_GE(0.00001, diff) << "Result: " << r << " Expect: " << e;
}

INSTANTIATE_TEST_CASE_P(ReadFloatPerLine, TestMathNewtonPoly, testing::ValuesIn(inputs));

namespace std {

    std::istream& operator>>(std::istream& in, FloatVector& pair) {
        std::string line;
        std::getline(in, line);
        std::stringstream ss(line);
        Float temp;
        while (ss >> temp) {
            pair.push_back(temp);
        }
        return in;
    }

}

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    std::ifstream in("TestMathNewtonPoly2.txt");
    FloatVector temp;    

    while (in >> temp) {
        inputs.push_back(temp);
        temp.clear();
    }
    in.close();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}