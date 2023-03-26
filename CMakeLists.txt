cmake_minimum_required(VERSION 3.14)

project(cadapt)

find_package(Qt6 COMPONENTS Core)


######################
# == Unit testing == #
######################
include(FetchContent)
FetchContent_Declare(
    googletest
    URL "https://github.com/google/googletest/archive/refs/heads/main.zip"
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

file(GLOB TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
foreach(STANDARD cxx_std_20 cxx_std_23)
    message(STATUS "${PROJECT_NAME}_test_${STANDARD}")
    add_executable(${PROJECT_NAME}_test_${STANDARD} "${TEST_SOURCE_FILES}")
    target_link_libraries(${PROJECT_NAME}_test_${STANDARD} GTest::gtest_main)
    if(Qt6_FOUND)
        target_link_libraries(${PROJECT_NAME}_test_${STANDARD} Qt6::Core)
    endif()
    target_include_directories(${PROJECT_NAME}_test_${STANDARD} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_compile_features(${PROJECT_NAME}_test_${STANDARD} PRIVATE "${STANDARD}")
    gtest_discover_tests(${PROJECT_NAME}_test_${STANDARD})
endforeach()

######################
# == Benchmarking == #
######################
include(FetchContent)
FetchContent_Declare(
    benchmark
    URL "https://github.com/google/benchmark/archive/refs/heads/main.zip"
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(benchmark)

enable_testing()
include(GoogleTest)

file(GLOB TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/benchmark/*.cpp")
foreach(STANDARD cxx_std_20 cxx_std_23)
    message(STATUS "${PROJECT_NAME}_benchmark_${STANDARD}")
    add_executable(${PROJECT_NAME}_benchmark_${STANDARD} "${TEST_SOURCE_FILES}")
    target_link_libraries(${PROJECT_NAME}_benchmark_${STANDARD} benchmark::benchmark)
    if(Qt6_FOUND)
        target_link_libraries(${PROJECT_NAME}_benchmark_${STANDARD} Qt6::Core)
    endif()
    target_include_directories(${PROJECT_NAME}_benchmark_${STANDARD} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_compile_features(${PROJECT_NAME}_benchmark_${STANDARD} PRIVATE "${STANDARD}")
endforeach()
