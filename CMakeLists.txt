# Minimum cmake version and project name
cmake_minimum_required(VERSION 3.10)
project(PSI_projekt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define sources and executable
set(SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Link boost
find_package(Boost 1.73.0 COMPONENTS filesystem system REQUIRED)

# Include Boost headers
if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found")
endif()


# Link Googletest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

# Set unit test sources, add an executable for them and link them to gtest
set(UNIT_TEST_SOURCES src/tests/ExampleTest.cpp)
add_executable(all-unit-tests ${UNIT_TEST_SOURCES})
target_link_libraries(all-unit-tests PUBLIC gtest_main)
