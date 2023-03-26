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


template <typename ... Ts>
static constexpr int test_algorithm(Ts&& ...) {
    return 42;
}

template <typename ... Ts>
static int test_algorithm(std::execution::unsequenced_policy, Ts&& ...) {
    return 4242;
}

TEST(utility, unseq_invoke) {
    using cadapt::unseq_invoke;

    constexpr auto test_algorithm_wrapper =
        []<typename ... T>(T&& ... v) {
            return test_algorithm(std::forward<T>(v) ...);
        };

    static_assert(unseq_invoke(test_algorithm_wrapper) == 42);
    static_assert(unseq_invoke(test_algorithm_wrapper, 6.6) == 42);
    static_assert(unseq_invoke(test_algorithm_wrapper, 'x', 3) == 42);

    EXPECT_EQ(unseq_invoke(test_algorithm_wrapper), 4242);
    EXPECT_EQ(unseq_invoke(test_algorithm_wrapper, 6.6), 4242);
    EXPECT_EQ(unseq_invoke(test_algorithm_wrapper, 'x', 3), 4242);
}
