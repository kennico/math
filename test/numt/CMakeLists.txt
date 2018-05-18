project(numt-test)
add_executable(numt-test
        EuclideanAlgoTest.cpp
        bigint/StringTest.cpp
        bigint/AdditionTest.cpp
        bigint/SubtractionTest.cpp
        CppTest.cpp
        bigint/AttributeTest.cpp)
target_link_libraries(numt-test ${TEST_LINK_LIBRARIES})
target_compile_definitions(numt-test PRIVATE KNY_MATH_DEBUG)