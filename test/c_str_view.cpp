#include <cadapt/c_str_view.hpp>

#include "test.hpp"

#include <concepts>

using namespace std::literals;
using namespace cadapt;


template <typename T>
static constexpr auto is_equal(
    std::basic_string_view<T> const ref,
    std::type_identity_t<T> const* c_str
) noexcept -> bool {
    return ref == c_str;
}

TEST(c_str_view_test, verify_c_str_ptr) {
    CT_EXPECT_NO_THROW(verify_c_str_ptr("", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(L"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(U"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u8"", 0));

    CT_EXPECT_NO_THROW(verify_c_str_ptr("test", 4));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(L"test", 4));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(U"test", 4));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u"test", 4));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u8"test", 4));

    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<wchar_t const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char32_t const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char16_t const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char8_t const*>(nullptr), 0));

    EXPECT_THROW(verify_c_str_ptr(static_cast<char const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<wchar_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<char32_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<char16_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<char8_t const*>(nullptr), 1), std::logic_error);
}

TEST(c_str_view_test, static_members) {
    static_assert(std::derived_from<c_str_view, std::string_view>);
    static_assert(std::derived_from<wc_str_view, std::wstring_view>);
    static_assert(std::derived_from<u32c_str_view, std::u32string_view>);
    static_assert(std::derived_from<u16c_str_view, std::u16string_view>);
    static_assert(std::derived_from<u8c_str_view, std::u8string_view>);

    struct traits: std::char_traits<char> {};
    static_assert(std::derived_from<basic_c_str_view<char, traits>, std::basic_string_view<char, traits>>);

    CT_EXPECT_TRUE(is_equal(""sv, c_str_view::empty));
    CT_EXPECT_TRUE(is_equal(L""sv, wc_str_view::empty));
    CT_EXPECT_TRUE(is_equal(U""sv, u32c_str_view::empty));
    CT_EXPECT_TRUE(is_equal(u""sv, u16c_str_view::empty));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view::empty));

    CT_EXPECT_TRUE(is_equal(""sv, c_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(L""sv, wc_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(U""sv, u32c_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(u""sv, u16c_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view::c_str_or_empty(nullptr)));

    CT_EXPECT_TRUE(is_equal("test"sv, c_str_view::c_str_or_empty("test")));
    CT_EXPECT_TRUE(is_equal(L"test"sv, wc_str_view::c_str_or_empty(L"test")));
    CT_EXPECT_TRUE(is_equal(U"test"sv, u32c_str_view::c_str_or_empty(U"test")));
    CT_EXPECT_TRUE(is_equal(u"test"sv, u16c_str_view::c_str_or_empty(u"test")));
    CT_EXPECT_TRUE(is_equal(u8"test"sv, u8c_str_view::c_str_or_empty(u8"test")));
}
