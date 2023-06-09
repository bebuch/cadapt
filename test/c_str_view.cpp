#include <cadapt/c_str_view.hpp>

#include "test.hpp"

#include <iostream>
#include <sstream>
#include <ranges>

using namespace std::literals;
using namespace cadapt;


template <typename T>
static constexpr void print_unequel(
    std::basic_string_view<T> const ref,
    std::type_identity_t<T> const* c_str
) noexcept {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << "0x";
    for(auto const& c: ref) {
        os << std::setw(sizeof(T) * 2) << static_cast<std::size_t>(c);
    }
    os << " != 0x";
    for(auto const& c: std::basic_string_view(c_str)) {
        os << std::setw(sizeof(T) * 2) << static_cast<std::size_t>(c);
    }
    os << '\n';
    std::cout << os.str();
}

template <typename T>
[[nodiscard]] static constexpr auto is_equal(
    std::basic_string_view<T> const ref,
    std::type_identity_t<T> const* c_str
) noexcept -> bool {
    if (ref == c_str) {
        return true;
    } else {
        if (!std::is_constant_evaluated()) {
            print_unequel(ref, c_str);
        }
        return false;
    }
}

template <typename T>
[[nodiscard]] static constexpr auto is_equal(
    std::basic_string_view<T> const ref,
    std::size_t const len,
    basic_c_str_view<T> const test
) noexcept -> bool {
    return len == test.size() && is_equal(ref, test.c_str());
}

template <typename T>
[[nodiscard]] static constexpr auto is_equal(
    std::basic_string_view<T> const ref,
    basic_c_str_view<T> const test
) noexcept -> bool {
    return ref == test && is_equal(ref, test.c_str());
}

template <typename T>
[[nodiscard]] static constexpr auto is_equal(std::nullptr_t, T const* c_str) noexcept -> bool {
    return nullptr == c_str;
}

template <typename T>
[[nodiscard]] static constexpr auto is_equal(
    std::nullptr_t,
    std::size_t const len,
    basic_c_str_view<T> const test
) noexcept -> bool {
    return len == test.size() && is_equal(nullptr, test.c_str());
}

template <typename C>
[[nodiscard]] static constexpr C const* ptr(C const* v) noexcept {
    return v;
};

template <typename C>
[[nodiscard]] static constexpr decltype(auto) non_const_ptr(C const* v) noexcept {
    return non_const(v);
};

TEST(c_str_view_test, verify_c_str_ptr) {
    CT_EXPECT_NO_THROW(verify_c_str_ptr("", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(L"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(U"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u8"", 0));

    CT_EXPECT_NO_THROW(verify_c_str_ptr("x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(L"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(U"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(u8"x", 1));

    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<wchar_t const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char32_t const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char16_t const*>(nullptr), 0));
    CT_EXPECT_NO_THROW(verify_c_str_ptr(static_cast<char8_t const*>(nullptr), 0));

    EXPECT_THROW((void)verify_c_str_ptr(static_cast<char const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)verify_c_str_ptr(static_cast<wchar_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)verify_c_str_ptr(static_cast<char32_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)verify_c_str_ptr(static_cast<char16_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)verify_c_str_ptr(static_cast<char8_t const*>(nullptr), 1), std::logic_error);
}

TEST(c_str_view_test, verify_c_str_data) {
    CT_EXPECT_NO_THROW(verify_c_str_data("", 0));
    CT_EXPECT_NO_THROW(verify_c_str_data(L"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_data(U"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_data(u"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_data(u8"", 0));

    CT_EXPECT_NO_THROW(verify_c_str_data("x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_data(L"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_data(U"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_data(u"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_data(u8"x", 1));

    EXPECT_THROW((void)verify_c_str_data("x\0y", 3), std::logic_error);
    EXPECT_THROW((void)verify_c_str_data(L"x\0y", 3), std::logic_error);
    EXPECT_THROW((void)verify_c_str_data(U"x\0y", 3), std::logic_error);
    EXPECT_THROW((void)verify_c_str_data(u"x\0y", 3), std::logic_error);
    EXPECT_THROW((void)verify_c_str_data(u8"x\0y", 3), std::logic_error);
}

TEST(c_str_view_test, verify_c_str_terminator) {
    CT_EXPECT_NO_THROW(verify_c_str_terminator("", 0));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(L"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(U"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(u"", 0));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(u8"", 0));

    CT_EXPECT_NO_THROW(verify_c_str_terminator("x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(L"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(U"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(u"x", 1));
    CT_EXPECT_NO_THROW(verify_c_str_terminator(u8"x", 1));

    EXPECT_THROW((void)verify_c_str_terminator("x", 0), std::logic_error);
    EXPECT_THROW((void)verify_c_str_terminator(L"x", 0), std::logic_error);
    EXPECT_THROW((void)verify_c_str_terminator(U"x", 0), std::logic_error);
    EXPECT_THROW((void)verify_c_str_terminator(u"x", 0), std::logic_error);
    EXPECT_THROW((void)verify_c_str_terminator(u8"x", 0), std::logic_error);
}

TEST(c_str_view_test, static_members) {
    static_assert(std::derived_from<c_str_view, std::string_view>);
    static_assert(std::derived_from<wc_str_view, std::wstring_view>);
    static_assert(std::derived_from<u32c_str_view, std::u32string_view>);
    static_assert(std::derived_from<u16c_str_view, std::u16string_view>);
    static_assert(std::derived_from<u8c_str_view, std::u8string_view>);

    struct traits: std::char_traits<char> {};
    static_assert(std::derived_from<basic_c_str_view<char, traits>, std::basic_string_view<char, traits>>);

    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view::empty));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view::empty));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view::empty));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view::empty));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view::empty));

    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view::c_str_or_empty(nullptr)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view::c_str_or_empty(nullptr)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view::c_str_or_empty("x")));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view::c_str_or_empty(L"x")));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view::c_str_or_empty(U"x")));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view::c_str_or_empty(u"x")));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view::c_str_or_empty(u8"x")));
}

TEST(c_str_view_test, constructor_default) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view()));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view()));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view()));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view()));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view()));
}

TEST(c_str_view_test, constructor_nullptr) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(nullptr)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(nullptr)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(nullptr)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(nullptr)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(nullptr)));
}

TEST(c_str_view_test, constructor_c_str) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(static_cast<char const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(static_cast<wchar_t const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(static_cast<char32_t const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(static_cast<char16_t const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(static_cast<char8_t const*>(nullptr))));

    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(ptr(""))));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(ptr(L""))));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(ptr(U""))));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(ptr(u""))));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(ptr(u8""))));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(ptr("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(ptr(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(ptr(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(ptr(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(ptr("x\0y"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(ptr(L"x\0y"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(ptr(U"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(ptr(u"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x\0y"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(non_const_ptr("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(non_const_ptr(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(non_const_ptr(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(non_const_ptr(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(non_const_ptr(u8"x"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(non_const_ptr("x\0y"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(non_const_ptr(L"x\0y"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(non_const_ptr(U"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(non_const_ptr(u"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(non_const_ptr(u8"x\0y"))));

    CT_EXPECT_TRUE(is_equal(""sv,   basic_c_str_view(static_cast<char const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(L""sv,  basic_c_str_view(static_cast<wchar_t const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(U""sv,  basic_c_str_view(static_cast<char32_t const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(u""sv,  basic_c_str_view(static_cast<char16_t const*>(nullptr))));
    CT_EXPECT_TRUE(is_equal(u8""sv, basic_c_str_view(static_cast<char8_t const*>(nullptr))));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(ptr("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(ptr(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(ptr(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(ptr(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(ptr(u8"x"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(ptr("x\0y"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(ptr(L"x\0y"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(ptr(U"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(ptr(u"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(ptr(u8"x\0y"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(non_const_ptr("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(non_const_ptr(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(non_const_ptr(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(non_const_ptr(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(non_const_ptr(u8"x"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(non_const_ptr("x\0y"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(non_const_ptr(L"x\0y"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(non_const_ptr(U"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(non_const_ptr(u"x\0y"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(non_const_ptr(u8"x\0y"))));
}

TEST(c_str_view_test, constructor_c_str_len) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(static_cast<char const*>(nullptr), 0)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(static_cast<wchar_t const*>(nullptr), 0)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(static_cast<char32_t const*>(nullptr), 0)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(static_cast<char16_t const*>(nullptr), 0)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(static_cast<char8_t const*>(nullptr), 0)));

    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(ptr(""), 0)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(ptr(L""), 0)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(ptr(U""), 0)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(ptr(u""), 0)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(ptr(u8""), 0)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(ptr("x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(ptr(L"x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(ptr(U"x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(ptr(u"x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x\0y"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(non_const_ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(non_const_ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(non_const_ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(non_const_ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(non_const_ptr(u8"x"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(ptr(u8"x"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(non_const_ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(non_const_ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(non_const_ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(non_const_ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(non_const_ptr(u8"x"), 1)));

    EXPECT_THROW((void)c_str_view(static_cast<char const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)wc_str_view(static_cast<wchar_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)u32c_str_view(static_cast<char32_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)u16c_str_view(static_cast<char16_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW((void)u8c_str_view(static_cast<char8_t const*>(nullptr), 1), std::logic_error);

    EXPECT_THROW((void)c_str_view(ptr("x"), 0), std::logic_error);
    EXPECT_THROW((void)wc_str_view(ptr(L"x"), 0), std::logic_error);
    EXPECT_THROW((void)u32c_str_view(ptr(U"x"), 0), std::logic_error);
    EXPECT_THROW((void)u16c_str_view(ptr(u"x"), 0), std::logic_error);
    EXPECT_THROW((void)u8c_str_view(ptr(u8"x"), 0), std::logic_error);

    EXPECT_THROW((void)c_str_view(ptr("x\0y"), 3), std::logic_error);
    EXPECT_THROW((void)wc_str_view(ptr(L"x\0y"), 3), std::logic_error);
    EXPECT_THROW((void)u32c_str_view(ptr(U"x\0y"), 3), std::logic_error);
    EXPECT_THROW((void)u16c_str_view(ptr(u"x\0y"), 3), std::logic_error);
    EXPECT_THROW((void)u8c_str_view(ptr(u8"x\0y"), 3), std::logic_error);
}

TEST(c_str_view_test, constructor_unverified_c_str_len) {
    EXPECT_TRUE(is_equal(nullptr, 0, c_str_view(null_term, static_cast<char const*>(nullptr), 0)));
    EXPECT_TRUE(is_equal(nullptr, 0, wc_str_view(null_term, static_cast<wchar_t const*>(nullptr), 0)));
    EXPECT_TRUE(is_equal(nullptr, 0, u32c_str_view(null_term, static_cast<char32_t const*>(nullptr), 0)));
    EXPECT_TRUE(is_equal(nullptr, 0, u16c_str_view(null_term, static_cast<char16_t const*>(nullptr), 0)));
    EXPECT_TRUE(is_equal(nullptr, 0, u8c_str_view(null_term, static_cast<char8_t const*>(nullptr), 0)));

    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(null_term, ptr(""), 0)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(null_term, ptr(L""), 0)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(null_term, ptr(U""), 0)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(null_term, ptr(u""), 0)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(null_term, ptr(u8""), 0)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(null_term, ptr("x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(null_term, ptr(L"x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(null_term, ptr(U"x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(null_term, ptr(u"x\0y"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(null_term, ptr(u8"x\0y"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(null_term, ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(null_term, ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(null_term, ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(null_term, ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(null_term, ptr(u8"x"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(null_term, non_const_ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(null_term, non_const_ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(null_term, non_const_ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(null_term, non_const_ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(null_term, non_const_ptr(u8"x"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(null_term, ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(null_term, ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(null_term, ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(null_term, ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(null_term, ptr(u8"x"), 1)));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(null_term, non_const_ptr("x"), 1)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(null_term, non_const_ptr(L"x"), 1)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(null_term, non_const_ptr(U"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(null_term, non_const_ptr(u"x"), 1)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(null_term, non_const_ptr(u8"x"), 1)));

    EXPECT_TRUE(is_equal(nullptr, 1, c_str_view(null_term, static_cast<char const*>(nullptr), 1)));
    EXPECT_TRUE(is_equal(nullptr, 1, wc_str_view(null_term, static_cast<wchar_t const*>(nullptr), 1)));
    EXPECT_TRUE(is_equal(nullptr, 1, u32c_str_view(null_term, static_cast<char32_t const*>(nullptr), 1)));
    EXPECT_TRUE(is_equal(nullptr, 1, u16c_str_view(null_term, static_cast<char16_t const*>(nullptr), 1)));
    EXPECT_TRUE(is_equal(nullptr, 1, u8c_str_view(null_term, static_cast<char8_t const*>(nullptr), 1)));

    EXPECT_TRUE(is_equal("x"sv,   0, c_str_view(null_term, ptr("x"), 0)));
    EXPECT_TRUE(is_equal(L"x"sv,  0, wc_str_view(null_term, ptr(L"x"), 0)));
    EXPECT_TRUE(is_equal(U"x"sv,  0, u32c_str_view(null_term, ptr(U"x"), 0)));
    EXPECT_TRUE(is_equal(u"x"sv,  0, u16c_str_view(null_term, ptr(u"x"), 0)));
    EXPECT_TRUE(is_equal(u8"x"sv, 0, u8c_str_view(null_term, ptr(u8"x"), 0)));

    EXPECT_TRUE(is_equal("x"sv,   3, c_str_view(null_term, ptr("x\0y"), 3)));
    EXPECT_TRUE(is_equal(L"x"sv,  3, wc_str_view(null_term, ptr(L"x\0y"), 3)));
    EXPECT_TRUE(is_equal(U"x"sv,  3, u32c_str_view(null_term, ptr(U"x\0y"), 3)));
    EXPECT_TRUE(is_equal(u"x"sv,  3, u16c_str_view(null_term, ptr(u"x\0y"), 3)));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, u8c_str_view(null_term, ptr(u8"x\0y"), 3)));
}

TEST(c_str_view_test, constructor_c_array) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view("")));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(L"")));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(U"")));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(u"")));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(u8"")));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view("x")));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(L"x")));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(U"x")));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(u"x")));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(u8"x")));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(non_const("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(non_const(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(non_const(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(non_const(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(non_const(u8"x"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view("x")));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(L"x")));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(U"x")));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(u"x")));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(u8"x")));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(non_const("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(non_const(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(non_const(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(non_const(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(non_const(u8"x"))));

    EXPECT_THROW((void)c_str_view("x\0y"), std::logic_error);
    EXPECT_THROW((void)wc_str_view(L"x\0y"), std::logic_error);
    EXPECT_THROW((void)u32c_str_view(U"x\0y"), std::logic_error);
    EXPECT_THROW((void)u16c_str_view(u"x\0y"), std::logic_error);
    EXPECT_THROW((void)u8c_str_view(u8"x\0y"), std::logic_error);

    EXPECT_THROW((void)c_str_view(non_const("x\0y")), std::logic_error);
    EXPECT_THROW((void)wc_str_view(non_const(L"x\0y")), std::logic_error);
    EXPECT_THROW((void)u32c_str_view(non_const(U"x\0y")), std::logic_error);
    EXPECT_THROW((void)u16c_str_view(non_const(u"x\0y")), std::logic_error);
    EXPECT_THROW((void)u8c_str_view(non_const(u8"x\0y")), std::logic_error);

    EXPECT_THROW((void)basic_c_str_view("x\0y"), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(L"x\0y"), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(U"x\0y"), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(u"x\0y"), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(u8"x\0y"), std::logic_error);

    EXPECT_THROW((void)basic_c_str_view(non_const("x\0y")), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(non_const(L"x\0y")), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(non_const(U"x\0y")), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(non_const(u"x\0y")), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(non_const(u8"x\0y")), std::logic_error);
}

TEST(c_str_view_test, constructor_unverified_c_array) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(null_term, "")));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(null_term, L"")));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(null_term, U"")));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(null_term, u"")));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(null_term, u8"")));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(null_term, "x")));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(null_term, L"x")));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(null_term, U"x")));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(null_term, u"x")));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(null_term, u8"x")));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(null_term, non_const("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(null_term, non_const(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(null_term, non_const(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(null_term, non_const(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(null_term, non_const(u8"x"))));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(null_term, "x")));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(null_term, L"x")));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(null_term, U"x")));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(null_term, u"x")));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(null_term, u8"x")));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(null_term, non_const("x"))));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(null_term, non_const(L"x"))));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(null_term, non_const(U"x"))));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(null_term, non_const(u"x"))));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(null_term, non_const(u8"x"))));

    EXPECT_TRUE(is_equal("x"sv,   3, c_str_view(null_term, "x\0y")));
    EXPECT_TRUE(is_equal(L"x"sv,  3, wc_str_view(null_term, L"x\0y")));
    EXPECT_TRUE(is_equal(U"x"sv,  3, u32c_str_view(null_term, U"x\0y")));
    EXPECT_TRUE(is_equal(u"x"sv,  3, u16c_str_view(null_term, u"x\0y")));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, u8c_str_view(null_term, u8"x\0y")));

    EXPECT_TRUE(is_equal("x"sv,   3, c_str_view(null_term, non_const("x\0y"))));
    EXPECT_TRUE(is_equal(L"x"sv,  3, wc_str_view(null_term, non_const(L"x\0y"))));
    EXPECT_TRUE(is_equal(U"x"sv,  3, u32c_str_view(null_term, non_const(U"x\0y"))));
    EXPECT_TRUE(is_equal(u"x"sv,  3, u16c_str_view(null_term, non_const(u"x\0y"))));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, u8c_str_view(null_term, non_const(u8"x\0y"))));

    EXPECT_TRUE(is_equal("x"sv,   3, basic_c_str_view(null_term, "x\0y")));
    EXPECT_TRUE(is_equal(L"x"sv,  3, basic_c_str_view(null_term, L"x\0y")));
    EXPECT_TRUE(is_equal(U"x"sv,  3, basic_c_str_view(null_term, U"x\0y")));
    EXPECT_TRUE(is_equal(u"x"sv,  3, basic_c_str_view(null_term, u"x\0y")));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, basic_c_str_view(null_term, u8"x\0y")));

    EXPECT_TRUE(is_equal("x"sv,   3, basic_c_str_view(null_term, non_const("x\0y"))));
    EXPECT_TRUE(is_equal(L"x"sv,  3, basic_c_str_view(null_term, non_const(L"x\0y"))));
    EXPECT_TRUE(is_equal(U"x"sv,  3, basic_c_str_view(null_term, non_const(U"x\0y"))));
    EXPECT_TRUE(is_equal(u"x"sv,  3, basic_c_str_view(null_term, non_const(u"x\0y"))));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, basic_c_str_view(null_term, non_const(u8"x\0y"))));
}

TEST(c_str_view_test, constructor_std_string) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(""s)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(L""s)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(U""s)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(u""s)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(u8""s)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view("x"s)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(L"x"s)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(U"x"s)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(u"x"s)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(u8"x"s)));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view("x"s)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(L"x"s)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(U"x"s)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(u"x"s)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(u8"x"s)));

    EXPECT_THROW((void)c_str_view("x\0y"s), std::logic_error);
    EXPECT_THROW((void)wc_str_view(L"x\0y"s), std::logic_error);
    EXPECT_THROW((void)u32c_str_view(U"x\0y"s), std::logic_error);
    EXPECT_THROW((void)u16c_str_view(u"x\0y"s), std::logic_error);
    EXPECT_THROW((void)u8c_str_view(u8"x\0y"s), std::logic_error);

    EXPECT_THROW((void)basic_c_str_view("x\0y"s), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(L"x\0y"s), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(U"x\0y"s), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(u"x\0y"s), std::logic_error);
    EXPECT_THROW((void)basic_c_str_view(u8"x\0y"s), std::logic_error);
}

TEST(c_str_view_test, constructor_unverified_std_string) {
    CT_EXPECT_TRUE(is_equal(""sv,   c_str_view(null_term, ""s)));
    CT_EXPECT_TRUE(is_equal(L""sv,  wc_str_view(null_term, L""s)));
    CT_EXPECT_TRUE(is_equal(U""sv,  u32c_str_view(null_term, U""s)));
    CT_EXPECT_TRUE(is_equal(u""sv,  u16c_str_view(null_term, u""s)));
    CT_EXPECT_TRUE(is_equal(u8""sv, u8c_str_view(null_term, u8""s)));

    CT_EXPECT_TRUE(is_equal("x"sv,   c_str_view(null_term, "x"s)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  wc_str_view(null_term, L"x"s)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  u32c_str_view(null_term, U"x"s)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  u16c_str_view(null_term, u"x"s)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, u8c_str_view(null_term, u8"x"s)));

    CT_EXPECT_TRUE(is_equal("x"sv,   basic_c_str_view(null_term, "x"s)));
    CT_EXPECT_TRUE(is_equal(L"x"sv,  basic_c_str_view(null_term, L"x"s)));
    CT_EXPECT_TRUE(is_equal(U"x"sv,  basic_c_str_view(null_term, U"x"s)));
    CT_EXPECT_TRUE(is_equal(u"x"sv,  basic_c_str_view(null_term, u"x"s)));
    CT_EXPECT_TRUE(is_equal(u8"x"sv, basic_c_str_view(null_term, u8"x"s)));

    EXPECT_TRUE(is_equal("x"sv,   3, c_str_view(null_term, "x\0y"s)));
    EXPECT_TRUE(is_equal(L"x"sv,  3, wc_str_view(null_term, L"x\0y"s)));
    EXPECT_TRUE(is_equal(U"x"sv,  3, u32c_str_view(null_term, U"x\0y"s)));
    EXPECT_TRUE(is_equal(u"x"sv,  3, u16c_str_view(null_term, u"x\0y"s)));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, u8c_str_view(null_term, u8"x\0y"s)));

    EXPECT_TRUE(is_equal("x"sv,   3, basic_c_str_view(null_term, "x\0y"s)));
    EXPECT_TRUE(is_equal(L"x"sv,  3, basic_c_str_view(null_term, L"x\0y"s)));
    EXPECT_TRUE(is_equal(U"x"sv,  3, basic_c_str_view(null_term, U"x\0y"s)));
    EXPECT_TRUE(is_equal(u"x"sv,  3, basic_c_str_view(null_term, u"x\0y"s)));
    EXPECT_TRUE(is_equal(u8"x"sv, 3, basic_c_str_view(null_term, u8"x\0y"s)));
}

TEST(c_str_view_test, max_size) {
    CT_EXPECT_TRUE(  ""sv.max_size() - 1 ==   ""_sv.max_size());
    CT_EXPECT_TRUE( L""sv.max_size() - 1 ==  L""_sv.max_size());
    CT_EXPECT_TRUE( U""sv.max_size() - 1 ==  U""_sv.max_size());
    CT_EXPECT_TRUE( u""sv.max_size() - 1 ==  u""_sv.max_size());
    CT_EXPECT_TRUE(u8""sv.max_size() - 1 == u8""_sv.max_size());
}

TEST(c_str_view_test, swap) {
    auto a = "a"_sv;
    auto b = "b"_sv;
    swap(a, b);
    EXPECT_EQ("b"sv, a);
    EXPECT_EQ("a"sv, b);

    static constexpr auto pair = []{
        auto a = "a"_sv;
        auto b = "b"_sv;
        swap(a, b);
        return std::make_pair(a, b);
    }();
    static_assert("b"sv == pair.first);
    static_assert("a"sv == pair.second);
}

TEST(c_str_view_test, ranges_support) {
    auto ref = "bcd"sv;
    auto test = "abc"_sv | std::views::transform([](char c) { return ++c; });
    ASSERT_EQ(ref.size(), std::size(test));
    for(std::size_t i = 0; i < ref.size(); ++i) {
        EXPECT_EQ(ref[i], test[i]);
    }
}
