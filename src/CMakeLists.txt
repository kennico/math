include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES numt.cpp)

add_library(math STATIC ${SOURCES})