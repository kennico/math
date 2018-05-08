#include <gtest/gtest.h>
#include <list>
#include <vector>

#include "numt/numt.h"

static const std::list<std::vector<int>> data = {
        {1317, 6415, 7791, 8037, 7424, 2641, 4226, 200},
        {6646, 8985, 7156, 5504, 1129, 2010, 2852, 6247},
        {8514, 1877, 5637, 5666, 8144, 2866, 9865, 2334},
        {3614, 8031, 9660, 6164, 4478, 8159, 5901, 8947},
        {6181, 1171, 3312, 6954, 8711, 8725, 7683, 2516},
        {2293, 3065, 7864, 9861, 2454, 5565, 8167, 9305},
        {1472, 6278, 3414, 9961, 3497, 2847, 5639, 676},
        {3650, 8338, 6382, 7546, 6794, 1356, 6761, 6393},
        {2269, 3029, 4881, 7236, 6155, 2779, 4300, 6411},
        {3934, 8309, 124,  9328, 7506, 3987, 8878, 369},
        {8793, 699,  2753, 1386, 5374, 3608, 1766, 6506},
        {4119, 5830, 4666, 5259, 6172, 9801, 4139, 8097},
        {1373, 8301, 9920, 5867, 7776, 6522, 1419, 3134},
        {3955, 4093, 9021, 7919, 1310, 9299, 3390, 1303},
        {4638, 4981, 1983, 6851, 8913, 7461, 2399, 7490},
        {9051, 2756, 9016, 20,   5668, 7864, 5814, 4607}
};


using namespace knylaw::math;

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