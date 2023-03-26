#include <cadapt/c_str.hpp>

#include <gtest/gtest.h>

#include <string_view>

using cadapt::c_str;
using cadapt::unverified_c_str;
using namespace std::literals;
using namespace cadapt::literals;

template <typename T>
static constexpr auto is_equal(
    std::basic_string_view<T> const ref,
    std::type_identity_t<T> const* c_str
) noexcept -> bool {
    return ref == c_str;
}

TEST(c_str_test, c_str) {
    EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test")));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st")));
    EXPECT_TRUE(is_equal("test"sv, c_str("test")));
    EXPECT_THROW(is_equal("te\0st"sv, c_str("te\0st")), std::logic_error);
}

TEST(c_str_test, std_string) {
    EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test"s)));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st"s)));
    EXPECT_TRUE(is_equal("test"sv, c_str("test"s)));
    EXPECT_THROW(is_equal("te\0st"sv, c_str("te\0st"s)), std::logic_error);
}

TEST(c_str_test, std_string_view) {
    EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test"sv)));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st"sv)));
    EXPECT_TRUE(is_equal("test"sv, c_str("test"sv)));
    EXPECT_THROW(is_equal("te\0st"sv, c_str("te\0st"sv)), std::logic_error);
}

TEST(c_str_test, c_str_view) {
    EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test"_sv)));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st"_sv)));
    EXPECT_TRUE(is_equal("test"sv, c_str("test"_sv)));
    EXPECT_THROW(is_equal("te\0st"sv, c_str("te\0st"_sv)), std::logic_error);
}

#if __has_include(<QString>)
TEST(c_str_test, qstring) {
    EXPECT_TRUE(is_equal("test"sv, unverified_c_str(QString::fromStdString("test"s))));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str(QString::fromStdString("te\0st"s))));
    EXPECT_TRUE(is_equal("test"sv, c_str(QString::fromStdString("test"s))));
    EXPECT_THROW(is_equal("te\0st"sv, c_str(QString::fromStdString("te\0st"s))), std::logic_error);
}
#endif
