#include "pch.h"
#include <fstream>

#include "utils.h"
#include "numt.h"
#pragma comment(lib, "math.lib")


using IntegerVector = std::vector<int>;

std::list<IntegerVector> numbers;

class MathgcdmTest :
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

TEST_P(MathgcdmTest, ExEuclideanAlgoForMultipleIntegers) {
    auto aa = GetParam();
    
    auto g = 0;
    IntegerVector xx;   
    auto t = knylaw::timing::measure([&]() {
        g = knylaw::math::gcdm(aa.cbegin(), aa.cend(), std::back_inserter(xx));
    }).count();
    std::cout << t << "ms used" << std::endl;

    auto r = dot(aa.cbegin(), aa.cend(), xx.cbegin());
    EXPECT_EQ(g, r);
}



INSTANTIATE_TEST_CASE_P(ReadFromFilePerLine, MathgcdmTest, ::testing::ValuesIn(numbers));

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
}

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    std::ifstream input("ExEuclideanAlgoForMultipleIntegers.txt");
    IntegerVector temp;
    while (input >> temp) {
        numbers.push_back(temp);
        temp.clear();
    }
    input.close();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}