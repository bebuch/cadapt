cmake_minimum_required(VERSION 3.14)

project(cadapt)

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

set(STANDARDS )

file(GLOB TEST_SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
foreach(STANDARD cxx_std_20 cxx_std_23)
    message(STATUS "${PROJECT_NAME}_tests_${STANDARD}")
    add_executable(${PROJECT_NAME}_tests_${STANDARD} "${TEST_SOURCE_FILES}")
    target_link_libraries(${PROJECT_NAME}_tests_${STANDARD} GTest::gtest_main)
    target_include_directories(${PROJECT_NAME}_tests_${STANDARD} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_compile_features(${PROJECT_NAME}_tests_${STANDARD} PRIVATE "${STANDARD}")
    gtest_discover_tests(${PROJECT_NAME}_tests_${STANDARD})
endforeach()
