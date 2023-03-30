#include <cadapt/c_str_view.hpp>

#include "test.hpp"

#include <concepts>
#include <sstream>
#include <iostream>

using namespace std::literals;
using namespace cadapt;


template <typename T>
static constexpr void print_unequel(
    std::basic_string_view<T> const ref,
    std::type_identity_t<T> const* c_str
) noexcept {
    std::ostringstream os;
    os << std::hex << std::setfill('0');
    for(auto const& c: ref) {
        os << std::setw(sizeof(T) * 2) << static_cast<std::size_t>(c);
    }
    os << " != ";
    for(auto const& c: std::basic_string_view(c_str)) {
        os << std::setw(sizeof(T) * 2) << static_cast<std::size_t>(c);
    }
    os << '\n';
    std::cout << os.str();
}


template <typename T>
static constexpr auto is_equal(
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
static constexpr decltype(auto) constless(T&& v) noexcept {
    return without_const(std::forward<T>(v));
};

template <typename C>
static constexpr C const* ptr(C const* v) noexcept {
    return v;
};

template <typename C>
static constexpr decltype(auto) constless_ptr(C const* v) noexcept {
    return constless(ptr(v));
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

    EXPECT_THROW(verify_c_str_ptr(static_cast<char const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<wchar_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<char32_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<char16_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(verify_c_str_ptr(static_cast<char8_t const*>(nullptr), 1), std::logic_error);
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

    EXPECT_THROW(verify_c_str_data("x\0y", 3), std::logic_error);
    EXPECT_THROW(verify_c_str_data(L"x\0y", 3), std::logic_error);
    EXPECT_THROW(verify_c_str_data(U"x\0y", 3), std::logic_error);
    EXPECT_THROW(verify_c_str_data(u"x\0y", 3), std::logic_error);
    EXPECT_THROW(verify_c_str_data(u8"x\0y", 3), std::logic_error);
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

    EXPECT_THROW(verify_c_str_terminator("x", 0), std::logic_error);
    EXPECT_THROW(verify_c_str_terminator(L"x", 0), std::logic_error);
    EXPECT_THROW(verify_c_str_terminator(U"x", 0), std::logic_error);
    EXPECT_THROW(verify_c_str_terminator(u"x", 0), std::logic_error);
    EXPECT_THROW(verify_c_str_terminator(u8"x", 0), std::logic_error);
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
    CT_EXPECT_NO_THROW(is_equal(""sv,   c_str_view().c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  wc_str_view().c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  u32c_str_view().c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  u16c_str_view().c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, u8c_str_view().c_str()));
}

TEST(c_str_view_test, constructor_nullptr) {
    CT_EXPECT_NO_THROW(is_equal(""sv,   c_str_view(nullptr).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  wc_str_view(nullptr).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  u32c_str_view(nullptr).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  u16c_str_view(nullptr).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, u8c_str_view(nullptr).c_str()));
}

TEST(c_str_view_test, constructor_c_str) {
    CT_EXPECT_NO_THROW(is_equal(""sv,   c_str_view(static_cast<char const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  wc_str_view(static_cast<wchar_t const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  u32c_str_view(static_cast<char32_t const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  u16c_str_view(static_cast<char16_t const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, u8c_str_view(static_cast<char8_t const*>(nullptr)).c_str()));

    CT_EXPECT_NO_THROW(is_equal(""sv,   c_str_view(ptr("")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  wc_str_view(ptr(L"")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  u32c_str_view(ptr(U"")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  u16c_str_view(ptr(u"")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, u8c_str_view(ptr(u8"")).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   c_str_view(ptr("x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  wc_str_view(ptr(L"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  u32c_str_view(ptr(U"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  u16c_str_view(ptr(u"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x")).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   c_str_view(ptr("x\0y")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  wc_str_view(ptr(L"x\0y")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  u32c_str_view(ptr(U"x\0y")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  u16c_str_view(ptr(u"x\0y")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x\0y")).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   c_str_view(constless_ptr("x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  wc_str_view(constless_ptr(L"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  u32c_str_view(constless_ptr(U"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  u16c_str_view(constless_ptr(u"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, u8c_str_view(constless_ptr(u8"x")).c_str()));

    CT_EXPECT_NO_THROW(is_equal(""sv,   basic_c_str_view(static_cast<char const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  basic_c_str_view(static_cast<wchar_t const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  basic_c_str_view(static_cast<char32_t const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  basic_c_str_view(static_cast<char16_t const*>(nullptr)).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, basic_c_str_view(static_cast<char8_t const*>(nullptr)).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   basic_c_str_view(constless_ptr("x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  basic_c_str_view(constless_ptr(L"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  basic_c_str_view(constless_ptr(U"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  basic_c_str_view(constless_ptr(u"x")).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, basic_c_str_view(constless_ptr(u8"x")).c_str()));
}

TEST(c_str_view_test, constructor_c_str_len) {
    CT_EXPECT_NO_THROW(is_equal(""sv,   c_str_view(static_cast<char const*>(nullptr), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  wc_str_view(static_cast<wchar_t const*>(nullptr), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  u32c_str_view(static_cast<char32_t const*>(nullptr), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  u16c_str_view(static_cast<char16_t const*>(nullptr), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, u8c_str_view(static_cast<char8_t const*>(nullptr), 0).c_str()));

    CT_EXPECT_NO_THROW(is_equal(""sv,   c_str_view(ptr(""), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L""sv,  wc_str_view(ptr(L""), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U""sv,  u32c_str_view(ptr(U""), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u""sv,  u16c_str_view(ptr(u""), 0).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8""sv, u8c_str_view(ptr(u8""), 0).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   c_str_view(ptr("x\0y"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  wc_str_view(ptr(L"x\0y"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  u32c_str_view(ptr(U"x\0y"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  u16c_str_view(ptr(u"x\0y"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x\0y"), 1).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   c_str_view(ptr("x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  wc_str_view(ptr(L"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  u32c_str_view(ptr(U"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  u16c_str_view(ptr(u"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, u8c_str_view(ptr(u8"x"), 1).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   c_str_view(constless_ptr("x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  wc_str_view(constless_ptr(L"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  u32c_str_view(constless_ptr(U"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  u16c_str_view(constless_ptr(u"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, u8c_str_view(constless_ptr(u8"x"), 1).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   basic_c_str_view(ptr("x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  basic_c_str_view(ptr(L"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  basic_c_str_view(ptr(U"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  basic_c_str_view(ptr(u"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, basic_c_str_view(ptr(u8"x"), 1).c_str()));

    CT_EXPECT_NO_THROW(is_equal("x"sv,   basic_c_str_view(constless_ptr("x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(L"x"sv,  basic_c_str_view(constless_ptr(L"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(U"x"sv,  basic_c_str_view(constless_ptr(U"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u"x"sv,  basic_c_str_view(constless_ptr(u"x"), 1).c_str()));
    CT_EXPECT_NO_THROW(is_equal(u8"x"sv, basic_c_str_view(constless_ptr(u8"x"), 1).c_str()));

    EXPECT_THROW(c_str_view(static_cast<char const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(wc_str_view(static_cast<wchar_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(u32c_str_view(static_cast<char32_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(u16c_str_view(static_cast<char16_t const*>(nullptr), 1), std::logic_error);
    EXPECT_THROW(u8c_str_view(static_cast<char8_t const*>(nullptr), 1), std::logic_error);

    EXPECT_THROW(c_str_view(ptr("x"), 0), std::logic_error);
    EXPECT_THROW(wc_str_view(ptr(L"x"), 0), std::logic_error);
    EXPECT_THROW(u32c_str_view(ptr(U"x"), 0), std::logic_error);
    EXPECT_THROW(u16c_str_view(ptr(u"x"), 0), std::logic_error);
    EXPECT_THROW(u8c_str_view(ptr(u8"x"), 0), std::logic_error);

    EXPECT_THROW(c_str_view(ptr("x\0y"), 3), std::logic_error);
    EXPECT_THROW(wc_str_view(ptr(L"x\0y"), 3), std::logic_error);
    EXPECT_THROW(u32c_str_view(ptr(U"x\0y"), 3), std::logic_error);
    EXPECT_THROW(u16c_str_view(ptr(u"x\0y"), 3), std::logic_error);
    EXPECT_THROW(u8c_str_view(ptr(u8"x\0y"), 3), std::logic_error);
}
