//
// Created by kenny on 5/15/18.
//
#include <gtest/gtest.h>
#include <algorithm>
#include <utility>
#include <utils.h>
#include <fstream>
#include "numt/bigint.h"

using namespace knylaw::math;





TEST(CppTest, StdStringSubstrIndexOutOfRange) {
    std::string ss = "123456";
    ss.substr(0, 99);
}



TEST(CppTest, LambdaAsDefaultArgument) {
    std::string s = "                       \t\t\n12345";
    std::string a = "12345";

    knylaw::ltrim(s);

    EXPECT_EQ(s, a);
}

TEST(CppTest, stringToULCommonBase) {
    EXPECT_EQ(std::stoul("z", nullptr, 36), 35);
    EXPECT_EQ(std::stoul("2z1", nullptr, 36), 2 * 36 * 36 + 35 * 36 + 1);

    size_t idx = 0;
    auto i = std::stol("1234 5678", &idx);
    EXPECT_EQ(1234, i);
    EXPECT_EQ(4, idx);

}

//TEST(CppTest, ifstreamOpenFile) {
//    std::ifstream input("StringTest.in");
//    EXPECT_EQ(true, !input);
//}