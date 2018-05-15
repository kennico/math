project(numa-test)

add_executable(numa-test
        IntegralTest.cpp
        InterpolationTest.cpp
        CubicSplineTest.cpp
        NewtonPolyTest.cpp
        LagrangePolyTest.cpp
        SectionedCubicHerimiteTest.cpp
        )

target_link_libraries(numa-test ${TEST_LINK_LIBRARIES})


