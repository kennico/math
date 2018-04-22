#include "pch.h"
#include <fstream>

#include "utils.h"
#include "numt.h"
#pragma comment(lib, "math.lib")


using IntegerVector = std::vector<int>;

std::list<IntegerVector> numbers;

class TestMathgcdm :
    public ::testing::TestWithParam<IntegerVector> {

public:

};

template<typename InIt1,
    typename InIt2,
    typename Integer = typename InIt1::value_type>
Integer dot(InIt1 first1, InIt1 last1, InIt2 first2) {
    Integer ret = 0;
    knylaw::for_each(first1, last1, first2, [&](auto a, auto b) {
        ret += a * b;
    });
    return ret;
}

TEST_P(TestMathgcdm, ExEuclideanAlgoOnMultipleIntegers) {
    auto aa = GetParam();
    IntegerVector xx;
    
    auto g = 0;   
    auto t = knylaw::timing::measure([&]() {
        g = knylaw::math::gcdm(aa.cbegin(), aa.cend(), std::back_inserter(xx));
    }).count();
    std::cout << t << "ms used" << std::endl;

    auto r = dot(aa.cbegin(), aa.cend(), xx.cbegin());
    EXPECT_EQ(g, r);
}

TEST_P(TestMathgcdm, EuclideanAlgoOnMultipleIntegers) {
    auto aa = GetParam();
    IntegerVector xx;

    auto g = 0;
    auto t = knylaw::timing::measure([&]() {
        g = knylaw::math::gcdm(aa.begin(), aa.end());
    }).count();
    std::cout << t << "ms used" << std::endl;

    auto r = knylaw::math::gcdm(aa.cbegin(), aa.cend(), std::back_inserter(xx));
    EXPECT_EQ(g, r);
}

INSTANTIATE_TEST_CASE_P(ReadFromFilePerLine, TestMathgcdm, ::testing::ValuesIn(numbers));

namespace std {
    std::istream& operator>>(std::istream& in, IntegerVector& pair) {
        std::string line;
        std::getline(in, line);
        std::stringstream ss(line);
        int temp;
        while (ss >> temp) {
            pair.push_back(temp);
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const IntegerVector& pair) {
        out << "{ ";
        for (auto i : pair) {
            out << i << ", ";
        }
        out << " }";
        return out;
    }
}

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    std::ifstream input("TestMathGcdm.txt");
    IntegerVector temp;
    while (input >> temp) {
        numbers.push_back(temp);
        temp.clear();
    }
    input.close();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}