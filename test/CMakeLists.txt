project(test)

# googletest
find_package(GTest)
if (${GTest_FOUND})
    message(STATUS "Found googletest")
else (${GTest_FOUND})
    message(STATUS "Could not locate googletest")
    message(STATUS "You are supposed to manually configure following variables.")

    set(GTEST_INCLUDE_DIRS "$ENV{GTEST_INCLUDE_DIRS}")
    set(GTEST_LIBRARIES "$ENV{GTEST_LIBRARIES}")
    set(GTEST_MAIN_LIBRARIES "$ENV{GTEST_MAIN_LIBRARIES}")

    message(STATUS "GTEST_INCLUDE_DIRS: " ${GTEST_INCLUDE_DIRS})
    message(STATUS "GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
    message(STATUS "GTEST_MAIN_LIBRARIES: " ${GTEST_MAIN_LIBRARIES})
endif (${GTest_FOUND})

# find_package(GTest REQUIRED) # Won't work on windows. Environment variables are needed to be set manually.
include_directories(${GTEST_INCLUDE_DIRS})
# project include dir
include_directories(${CMAKE_SOURCE_DIR}/include)

if (WIN32)
    option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
    option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)
    set(TEST_LINK_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} gtest_main knymath)
else (WIN32)
    set(TEST_LINK_LIBRARIES ${GTEST_LIBRARIES} pthread gtest_main knymath)
endif (WIN32)

add_subdirectory(numa)
add_subdirectory(numt)