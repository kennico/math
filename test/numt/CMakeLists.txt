project(numt-test)

add_executable(numt-test EuclideanAlgoTest.cpp)
target_link_libraries(numt-test ${TEST_LINK_LIBRARIES})