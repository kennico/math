#include "gtest\gtest.h"

#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "math.lib")

int main(int argc, char* argv[], char* envp[]) {
    // TODO main
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}