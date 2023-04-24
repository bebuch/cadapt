cmake_minimum_required(VERSION 3.12)

project(cadapt)


# Build tests with Qt support of available
find_package(Qt6 COMPONENTS Core)


##################################################
# == Provide Google Test and Google Benchmark == #
##################################################

include(FetchContent)

FetchContent_Declare(
    googletest
    URL "https://github.com/google/googletest/archive/refs/heads/main.zip")

FetchContent_Declare(
    googlebenchmark
    URL "https://github.com/google/benchmark/archive/refs/heads/main.zip")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# disable building benchmark library tests
set(BENCHMARK_ENABLE_TESTING OFF)

# disable building benchmark library tests
set(CMAKE_CXX_STANDARD 20)

FetchContent_MakeAvailable(googletest googlebenchmark)

enable_testing()
include(GoogleTest)


# Create list of c++ standards supported by target_compile_features depending on CMake version
set(TESTED_STANDARDS cxx_std_20)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.20.0")
    list(APPEND TESTED_STANDARDS cxx_std_23)
endif()
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.25.0")
    list(APPEND TESTED_STANDARDS cxx_std_26)
endif()

######################
# == Unit testing == #
######################
file(GLOB TEST_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/test/*.cpp")
foreach(STANDARD ${TESTED_STANDARDS})
    add_executable(${PROJECT_NAME}_test_${STANDARD} "${TEST_SOURCE_FILES}")
    target_link_libraries(${PROJECT_NAME}_test_${STANDARD} GTest::gtest_main)
    if(Qt6_FOUND)
        target_link_libraries(${PROJECT_NAME}_test_${STANDARD} Qt6::Core)
    endif()
    target_include_directories(${PROJECT_NAME}_test_${STANDARD} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_compile_features(${PROJECT_NAME}_test_${STANDARD} PRIVATE "${STANDARD}")
    # Enable for debug purpose
    # add_compile_definitions(${PROJECT_NAME}_test_${STANDARD} DISABLE_CT_TESTS)
    gtest_discover_tests(${PROJECT_NAME}_test_${STANDARD})
endforeach()

######################
# == Benchmarking == #
######################
file(GLOB TEST_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/benchmark/*.cpp")
foreach(STANDARD ${TESTED_STANDARDS})
    add_executable(${PROJECT_NAME}_benchmark_${STANDARD} "${TEST_SOURCE_FILES}")
    target_link_libraries(${PROJECT_NAME}_benchmark_${STANDARD} benchmark::benchmark)
    if(Qt6_FOUND)
        target_link_libraries(${PROJECT_NAME}_benchmark_${STANDARD} Qt6::Core)
    endif()
    target_include_directories(${PROJECT_NAME}_benchmark_${STANDARD} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_compile_features(${PROJECT_NAME}_benchmark_${STANDARD} PRIVATE "${STANDARD}")
endforeach()
