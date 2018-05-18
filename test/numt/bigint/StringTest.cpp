//
// Created by kenny on 5/16/18.
//
#include <gtest/gtest.h>
#include <fstream>

#include "numt/bigint.h"
using namespace knylaw::math;


TEST(BigIntegerTest, String) {


    std::ifstream in0("./StringTest.in");
    std::string s;
    if (!in0) {
        throw std::runtime_error("File not opened.");
    }
    while (std::getline(in0, s)) {
        if (!s.empty()) {
            auto b = BigInteger::valueOf(s);
            EXPECT_EQ(s, to_string(b));
        }
    }
}


/*

import random
numeric = "1234567890"
maxlen = 1024
with open("StringTest.in", mode="w") as f:
    for i in range(64):
        l = random.randint(0, maxlen)
        string = "".join([random.choice(numeric) for j in range(0, l)]).lstrip("0")
        if string:
            sign = "-" if random.choice((True, False)) else ""
            string = sign + string
        else:
            string = "0"
        _ = f.write(string + "\n")

 */