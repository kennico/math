project(knymath)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES numt.cpp bigint.cpp)

add_library(knymath STATIC ${SOURCES})
target_compile_definitions(knymath PRIVATE KNY_MATH_DEBUG)