project(numt-test)
# googletest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
# project include dir
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(numt-test EuclideanAlgoTest.cpp)
target_link_libraries(numt-test ${GTEST_LIBRARIES} pthread gtest_main)