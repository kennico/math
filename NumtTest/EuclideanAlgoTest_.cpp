#include "EuclideanAlgoTest.h"

#pragma comment(lib, "math.lib")

using namespace knylaw::math;
using namespace knylaw::math::test;

class EuclideanAlgoTest :
    public ::testing::TestWithParam<std::vector<int>> {

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

TEST_P(EuclideanAlgoTest, MultipleIntegersExtended) {
    auto aa = GetParam();
    std::vector<int> xx;
    
    auto g = knylaw::math::gcdm(aa.cbegin(), aa.cend(), std::back_inserter(xx));   
    auto r = dot(aa.cbegin(), aa.cend(), xx.cbegin());

    EXPECT_EQ(g, r);
}

TEST_P(EuclideanAlgoTest, Extended) {
    auto aa = GetParam();
    std::vector<int> xx;

    auto g = knylaw::math::gcdm(aa.begin(), aa.end());
    auto r = knylaw::math::gcdm(aa.cbegin(), aa.cend(), std::back_inserter(xx));

    EXPECT_EQ(g, r);
}

INSTANTIATE_TEST_CASE_P(ReadFromFilePerLine, EuclideanAlgoTest, ::testing::ValuesIn(data));

