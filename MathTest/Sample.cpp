#include "pch.h"

std::vector<int> ints;

class CombinationsTest :
    public ::testing::TestWithParam<int> {
};

TEST_P(CombinationsTest, Basic) {
    std::cout << "int: " << GetParam()
        << "\"\n";
}

INSTANTIATE_TEST_CASE_P(AllCombinations,
    CombinationsTest,
    ::testing::ValuesIn(ints));

int main(int argc, char **argv) {
    for (int i = 0; i < 10; ++i) {
        ints.push_back(i * 100);
    }
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}