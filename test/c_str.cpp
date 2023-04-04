#include <cadapt/c_str.hpp>

#include "test.hpp"

using namespace std::literals;
using namespace cadapt;

template <typename T>
static constexpr auto is_equal(
    std::basic_string_view<T> const ref,
    std::type_identity_t<T> const* c_str
) noexcept -> bool {
    return ref == c_str;
}

TEST(c_str_test, c_str) {
    CT_EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test")));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st")));
    CT_EXPECT_TRUE(is_equal("test"sv, c_str("test")));
    EXPECT_THROW((void)is_equal("te\0st"sv, c_str("te\0st")), std::logic_error);

    CT_EXPECT_TRUE(is_equal(L"test"sv, unverified_c_str(L"test")));
    EXPECT_FALSE(is_equal(L"te\0st"sv, unverified_c_str(L"te\0st")));
    CT_EXPECT_TRUE(is_equal(L"test"sv, c_str(L"test")));
    EXPECT_THROW((void)is_equal(L"te\0st"sv, c_str(L"te\0st")), std::logic_error);

    CT_EXPECT_TRUE(is_equal(U"test"sv, unverified_c_str(U"test")));
    EXPECT_FALSE(is_equal(U"te\0st"sv, unverified_c_str(U"te\0st")));
    CT_EXPECT_TRUE(is_equal(U"test"sv, c_str(U"test")));
    EXPECT_THROW((void)is_equal(U"te\0st"sv, c_str(U"te\0st")), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u"test"sv, unverified_c_str(u"test")));
    EXPECT_FALSE(is_equal(u"te\0st"sv, unverified_c_str(u"te\0st")));
    CT_EXPECT_TRUE(is_equal(u"test"sv, c_str(u"test")));
    EXPECT_THROW((void)is_equal(u"te\0st"sv, c_str(u"te\0st")), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u8"test"sv, unverified_c_str(u8"test")));
    EXPECT_FALSE(is_equal(u8"te\0st"sv, unverified_c_str(u8"te\0st")));
    CT_EXPECT_TRUE(is_equal(u8"test"sv, c_str(u8"test")));
    EXPECT_THROW((void)is_equal(u8"te\0st"sv, c_str(u8"te\0st")), std::logic_error);
}

TEST(c_str_test, std_string) {
    CT_EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test"s)));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st"s)));
    CT_EXPECT_TRUE(is_equal("test"sv, c_str("test"s)));
    EXPECT_THROW((void)is_equal("te\0st"sv, c_str("te\0st"s)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(L"test"sv, unverified_c_str(L"test"s)));
    EXPECT_FALSE(is_equal(L"te\0st"sv, unverified_c_str(L"te\0st"s)));
    CT_EXPECT_TRUE(is_equal(L"test"sv, c_str(L"test"s)));
    EXPECT_THROW((void)is_equal(L"te\0st"sv, c_str(L"te\0st"s)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(U"test"sv, unverified_c_str(U"test"s)));
    EXPECT_FALSE(is_equal(U"te\0st"sv, unverified_c_str(U"te\0st"s)));
    CT_EXPECT_TRUE(is_equal(U"test"sv, c_str(U"test"s)));
    EXPECT_THROW((void)is_equal(U"te\0st"sv, c_str(U"te\0st"s)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u"test"sv, unverified_c_str(u"test"s)));
    EXPECT_FALSE(is_equal(u"te\0st"sv, unverified_c_str(u"te\0st"s)));
    CT_EXPECT_TRUE(is_equal(u"test"sv, c_str(u"test"s)));
    EXPECT_THROW((void)is_equal(u"te\0st"sv, c_str(u"te\0st"s)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u8"test"sv, unverified_c_str(u8"test"s)));
    EXPECT_FALSE(is_equal(u8"te\0st"sv, unverified_c_str(u8"te\0st"s)));
    CT_EXPECT_TRUE(is_equal(u8"test"sv, c_str(u8"test"s)));
    EXPECT_THROW((void)is_equal(u8"te\0st"sv, c_str(u8"te\0st"s)), std::logic_error);
}

TEST(c_str_test, std_string_view) {
    CT_EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test"sv)));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str("te\0st"sv)));
    CT_EXPECT_TRUE(is_equal("test"sv, c_str("test"sv)));
    EXPECT_THROW((void)is_equal("te\0st"sv, c_str("te\0st"sv)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(L"test"sv, unverified_c_str(L"test"sv)));
    EXPECT_FALSE(is_equal(L"te\0st"sv, unverified_c_str(L"te\0st"sv)));
    CT_EXPECT_TRUE(is_equal(L"test"sv, c_str(L"test"sv)));
    EXPECT_THROW((void)is_equal(L"te\0st"sv, c_str(L"te\0st"sv)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(U"test"sv, unverified_c_str(U"test"sv)));
    EXPECT_FALSE(is_equal(U"te\0st"sv, unverified_c_str(U"te\0st"sv)));
    CT_EXPECT_TRUE(is_equal(U"test"sv, c_str(U"test"sv)));
    EXPECT_THROW((void)is_equal(U"te\0st"sv, c_str(U"te\0st"sv)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u"test"sv, unverified_c_str(u"test"sv)));
    CT_EXPECT_FALSE(is_equal(u"te\0st"sv, unverified_c_str(u"te\0st"sv)));
    CT_EXPECT_TRUE(is_equal(u"test"sv, c_str(u"test"sv)));
    EXPECT_THROW((void)is_equal(u"te\0st"sv, c_str(u"te\0st"sv)), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u8"test"sv, unverified_c_str(u8"test"sv)));
    EXPECT_FALSE(is_equal(u8"te\0st"sv, unverified_c_str(u8"te\0st"sv)));
    CT_EXPECT_TRUE(is_equal(u8"test"sv, c_str(u8"test"sv)));
    EXPECT_THROW((void)is_equal(u8"te\0st"sv, c_str(u8"te\0st"sv)), std::logic_error);
}

TEST(c_str_test, c_str_view) {
    CT_EXPECT_TRUE(is_equal("test"sv, unverified_c_str("test"_sv)));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str(basic_c_str_view(null_term, "te\0st"))));
    CT_EXPECT_TRUE(is_equal("test"sv, c_str("test"_sv)));
    EXPECT_THROW((void)is_equal("te\0st"sv, c_str(basic_c_str_view(null_term, "te\0st"))), std::logic_error);

    CT_EXPECT_TRUE(is_equal(L"test"sv, unverified_c_str(L"test"_sv)));
    EXPECT_FALSE(is_equal(L"te\0st"sv, unverified_c_str(basic_c_str_view(null_term, L"te\0st"))));
    CT_EXPECT_TRUE(is_equal(L"test"sv, c_str(L"test"_sv)));
    EXPECT_THROW((void)is_equal(L"te\0st"sv, c_str(basic_c_str_view(null_term, L"te\0st"))), std::logic_error);

    CT_EXPECT_TRUE(is_equal(U"test"sv, unverified_c_str(U"test"_sv)));
    EXPECT_FALSE(is_equal(U"te\0st"sv, unverified_c_str(basic_c_str_view(null_term, U"te\0st"))));
    CT_EXPECT_TRUE(is_equal(U"test"sv, c_str(U"test"_sv)));
    EXPECT_THROW((void)is_equal(U"te\0st"sv, c_str(basic_c_str_view(null_term, U"te\0st"))), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u"test"sv, unverified_c_str(u"test"_sv)));
    EXPECT_FALSE(is_equal(u"te\0st"sv, unverified_c_str(basic_c_str_view(null_term, u"te\0st"))));
    CT_EXPECT_TRUE(is_equal(u"test"sv, c_str(u"test"_sv)));
    EXPECT_THROW((void)is_equal(u"te\0st"sv, c_str(basic_c_str_view(null_term, u"te\0st"))), std::logic_error);

    CT_EXPECT_TRUE(is_equal(u8"test"sv, unverified_c_str(u8"test"_sv)));
    EXPECT_FALSE(is_equal(u8"te\0st"sv, unverified_c_str(basic_c_str_view(null_term, u8"te\0st"))));
    CT_EXPECT_TRUE(is_equal(u8"test"sv, c_str(u8"test"_sv)));
    EXPECT_THROW((void)is_equal(u8"te\0st"sv, c_str(basic_c_str_view(null_term, u8"te\0st"))), std::logic_error);
}

#if __has_include(<QString>)
TEST(c_str_test, qstring) {
    EXPECT_TRUE(is_equal("test"sv, unverified_c_str(QString::fromStdString("test"s))));
    EXPECT_FALSE(is_equal("te\0st"sv, unverified_c_str(QString::fromStdString("te\0st"s))));
    EXPECT_TRUE(is_equal("test"sv, c_str(QString::fromStdString("test"s))));
    EXPECT_THROW((void)is_equal("te\0st"sv, c_str(QString::fromStdString("te\0st"s))), std::logic_error);

    EXPECT_TRUE(is_equal(L"test"sv, unverified_c_str<wchar_t>(QString::fromStdString("test"s))));
    EXPECT_FALSE(is_equal(L"te\0st"sv, unverified_c_str<wchar_t>(QString::fromStdString("te\0st"s))));
    EXPECT_TRUE(is_equal(L"test"sv, c_str<wchar_t>(QString::fromStdString("test"s))));
    EXPECT_THROW((void)is_equal(L"te\0st"sv, c_str<wchar_t>(QString::fromStdString("te\0st"s))), std::logic_error);

    EXPECT_TRUE(is_equal(U"test"sv, unverified_c_str<char32_t>(QString::fromStdString("test"s))));
    EXPECT_FALSE(is_equal(U"te\0st"sv, unverified_c_str<char32_t>(QString::fromStdString("te\0st"s))));
    EXPECT_TRUE(is_equal(U"test"sv, c_str<char32_t>(QString::fromStdString("test"s))));
    EXPECT_THROW((void)is_equal(U"te\0st"sv, c_str<char32_t>(QString::fromStdString("te\0st"s))), std::logic_error);

    EXPECT_TRUE(is_equal(u"test"sv, unverified_c_str<char16_t>(QString::fromStdString("test"s))));
    EXPECT_FALSE(is_equal(u"te\0st"sv, unverified_c_str<char16_t>(QString::fromStdString("te\0st"s))));
    EXPECT_TRUE(is_equal(u"test"sv, c_str<char16_t>(QString::fromStdString("test"s))));
    EXPECT_THROW((void)is_equal(u"te\0st"sv, c_str<char16_t>(QString::fromStdString("te\0st"s))), std::logic_error);
}
#endif
