#include "pch.h"
#include <fstream>

#include "utils.h"
#include "numt.h"
#pragma comment(lib, "math.lib")


using IntegerPair = std::pair<int, int>;

std::list<IntegerPair> numbers;

class TestMathgcd :
    public ::testing::TestWithParam<IntegerPair> {

public:

};

TEST_P(TestMathgcd, ExEuclideanAlgoOnIntegerPairs) {
    auto aa = GetParam();
    auto x = 0, y = 0;
    auto g = knylaw::math::gcd(aa.first, aa.second, &x, &y);
    EXPECT_EQ(g, aa.first*x + aa.second*y);
}

TEST_P(TestMathgcd, EuclideanAlgoOnIntegerPairs) {
    auto aa = GetParam();
    auto x = 0;
    auto g = knylaw::math::gcd(aa.first, aa.second);
    // Explicit type argument is needed to convert NULL to int*
    auto r = knylaw::math::gcd<int>(aa.first, aa.second, NULL, NULL);

    EXPECT_EQ(g, r);
}

INSTANTIATE_TEST_CASE_P(ReadFromFilePerLine, TestMathgcd, ::testing::ValuesIn(numbers));

namespace std {
    std::istream& operator>>(std::istream& in, IntegerPair& pair) {
        in >> pair.first >> pair.second;
        return in;
    }
}

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    std::ifstream input("TestMathGcd.txt");
    IntegerPair temp;
    while (input >> temp) {
        numbers.push_back(temp);
    }
    input.close();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}