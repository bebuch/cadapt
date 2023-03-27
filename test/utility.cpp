#include <cadapt/utility.hpp>

#include <gtest/gtest.h>


static_assert(!cadapt::same_as_one_of<int>);
static_assert(!cadapt::same_as_one_of<int, char>);
static_assert(!cadapt::same_as_one_of<int, float, long>);
static_assert(cadapt::same_as_one_of<int, int>);
static_assert(cadapt::same_as_one_of<int, int, long>);
static_assert(cadapt::same_as_one_of<int, float, int>);


template <typename T>
static constexpr bool is_equal(T const* ref, T* test) noexcept {
    return ref == test;
}

TEST(utility, without_const) {
    using cadapt::without_const;

    static constexpr int value = 5;
    static_assert(is_equal(&value, without_const(&value)));

    char const* text = "test";
    EXPECT_TRUE(is_equal(text, without_const(text)));
    EXPECT_TRUE(is_equal(&value, without_const(&value)));
}
