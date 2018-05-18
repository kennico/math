//
// Created by kenny on 5/19/18.
//

#include <gtest/gtest.h>



#include "numt/bigint.h"
using namespace knylaw::math;

BigInteger::DgtStorage
        a = {2,2,2,2,2}, b = {1,1,1,1,1,1,3},
        c = {1,9,4}, d = {8, 2, 2,2,2,2,2},
        e = {5,6,7,8}, f = {7,7};

TEST(BigIntegerTest, VectorLessThanOperator) {
    EXPECT_EQ((a < b), 0);
    EXPECT_EQ((c < d), 1); //
    EXPECT_EQ((e < f), 1);
}

TEST(BigIntegerTest, DigitsContainerCompare) {
    EXPECT_EQ(BigInteger::dgtLessThan(a, b), 1);
    EXPECT_EQ(BigInteger::dgtLessThan(c, d), 1);
    EXPECT_EQ(BigInteger::dgtLessThan(e, f), 0);
}