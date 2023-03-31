#include <cadapt/utility.hpp>

#include "test.hpp"


static_assert(!cadapt::same_as_one_of<int>);
static_assert(!cadapt::same_as_one_of<int, char>);
static_assert(!cadapt::same_as_one_of<int, float, long>);
static_assert(cadapt::same_as_one_of<int, int>);
static_assert(cadapt::same_as_one_of<int, int, long>);
static_assert(cadapt::same_as_one_of<int, float, int>);


template <typename Ref, typename Test>
[[nodiscard]] static constexpr bool is_equal(Ref&& ref, Test&& test) noexcept {
    return std::forward<Ref>(ref) == std::forward<Test>(test);
}

TEST(utility, non_const) {
    using cadapt::non_const;

    static constexpr char data[] = "";
    static_assert(std::same_as<char*, decltype(non_const(static_cast<char const*>(&data[0])))>);
    static_assert(std::same_as<char&, decltype(non_const(static_cast<char const&>(data[0])))>);
    static_assert(std::same_as<char&&, decltype(non_const(static_cast<char const&&>(data[0])))>);
    static_assert(std::same_as<char*, decltype(non_const(const_cast<char*>(static_cast<char const*>(&data[0]))))>);
    static_assert(std::same_as<char&, decltype(non_const(const_cast<char&>(static_cast<char const&>(data[0]))))>);
    static_assert(std::same_as<char&&, decltype(non_const(const_cast<char&&>(static_cast<char const&&>(data[0]))))>);
    static_assert(std::same_as<char(*)[1], decltype(non_const(static_cast<char const(*)[1]>(&data)))>);
    static_assert(std::same_as<char(&)[1], decltype(non_const(static_cast<char const(&)[1]>(data)))>);
    static_assert(std::same_as<char(&&)[1], decltype(non_const(static_cast<char const(&&)[1]>(data)))>);
    static_assert(std::same_as<char(*)[1], decltype(non_const(const_cast<char(*)[1]>(static_cast<char const(*)[1]>(&data))))>);
    static_assert(std::same_as<char(&)[1], decltype(non_const(const_cast<char(&)[1]>(static_cast<char const(&)[1]>(data))))>);
    static_assert(std::same_as<char(&&)[1], decltype(non_const(const_cast<char(&&)[1]>(static_cast<char const(&&)[1]>(data))))>);
    static_assert(std::same_as<char(&)[2], decltype(non_const("x"))>);

    static constexpr int value = 5;
    CT_EXPECT_TRUE(is_equal(&value, non_const(&value)));

    static constexpr char const* text = "test";
    CT_EXPECT_TRUE(is_equal(text, non_const(text)));

    static constexpr char array[] = "test";
    CT_EXPECT_TRUE(is_equal(array, non_const(array)));
}
