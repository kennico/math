#include "common.h"

using knylaw::math::impl::BigInteger_1E4;

TEST(BigIntegerTest, toString) {
    BigInteger_1E4 a;
    EXPECT_EQ(a.str(), std::to_string(0));

    BigInteger_1E4 b(30541852);
    EXPECT_EQ(b.str(), std::to_string(30541852));
}