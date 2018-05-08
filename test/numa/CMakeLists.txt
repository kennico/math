project(numa-test)
# googletest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
# project include dir
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(numa-test
        IntegralTest.cpp
        InterpolationTest.cpp
        CubicSplineTest.cpp
        NewtonPolyTest.cpp
        LagrangePolyTest.cpp
        SectionedCubicHerimiteTest.cpp
        )
target_link_libraries(numa-test ${GTEST_LIBRARIES} pthread gtest_main)