#include <cadapt/inout_c_str.hpp>

#include "test.hpp"

using namespace std::literals;
using namespace cadapt;


TEST(inout_c_str, resize_for_out_c_str) {
    static constexpr auto test = []<typename T>(std::basic_string<T> target) {
        resize_for_out_c_str(target, 4);
        ASSERT_EQ(target.size(), 4);
        EXPECT_EQ(target[target.size()], 0);
        resize_for_out_c_str(target, 512);
        ASSERT_EQ(target.size(), 512);
        EXPECT_EQ(target[target.size()], 0);
    };

    test(std::string{});
    test(std::wstring{});
    test(std::u32string{});
    test(std::u16string{});
    test(std::u8string{});

    static constexpr auto ct_resize = []<typename T>(std::basic_string<T>&& target, std::size_t count) {
        resize_for_out_c_str(target, count);
        return std::make_pair(target[target.size()], target.size());
    };

    static_assert(ct_resize("test"s, 32) == std::make_pair('\0', std::size_t(32)));
    static_assert(ct_resize(L"test"s, 32) == std::make_pair(L'\0', std::size_t(32)));
    static_assert(ct_resize(U"test"s, 32) == std::make_pair(U'\0', std::size_t(32)));
    static_assert(ct_resize(u"test"s, 32) == std::make_pair(u'\0', std::size_t(32)));
    static_assert(ct_resize(u8"test"s, 32) == std::make_pair(u8'\0', std::size_t(32)));
}

TEST(inout_c_str, enlarge_for_inout_c_str) {
    static constexpr auto test = []<typename T>(std::basic_string<T> target) {
        auto const original_c_str = std::basic_string<T>(target.c_str());

        enlarge_for_inout_c_str(target, 32);
        ASSERT_EQ(target.size(), 32);
        ASSERT_EQ(original_c_str, target.c_str());
        enlarge_for_inout_c_str(target, 512);
        ASSERT_EQ(target.size(), 512);
        ASSERT_EQ(original_c_str, target.c_str());
        EXPECT_THROW(enlarge_for_inout_c_str(target, 384), std::logic_error);
    };

    test("test"s);
    test(L"test"s);
    test(U"test"s);
    test(u"test"s);
    test(u8"test"s);

    static constexpr auto ct_enlarge = []<typename T>(std::basic_string<T>&& target, std::size_t count) {
        enlarge_for_inout_c_str(target, count);
        return std::make_pair(std::basic_string_view(target.c_str()), target.size());
    };

    static_assert(ct_enlarge("test"s, 32) == std::make_pair("test"sv, std::size_t(32)));
    static_assert(ct_enlarge(L"test"s, 32) == std::make_pair(L"test"sv, std::size_t(32)));
    static_assert(ct_enlarge(U"test"s, 32) == std::make_pair(U"test"sv, std::size_t(32)));
    static_assert(ct_enlarge(u"test"s, 32) == std::make_pair(u"test"sv, std::size_t(32)));
    static_assert(ct_enlarge(u8"test"s, 32) == std::make_pair(u8"test"sv, std::size_t(32)));
}

template <typename C>
static constexpr void c_fn_no_change(C*, std::size_t) {}

template <typename C>
static constexpr void c_fn_abc(C* const buffer, std::size_t const buffer_size) {
    for(std::size_t i = 0; i < buffer_size; ++i) {
        buffer[i] = static_cast<C>('a' + i);
    }
}

template <typename C>
static constexpr void c_fn_plus1(C* const buffer, std::size_t const buffer_size) {
    for(std::size_t i = 0; i < buffer_size; ++i) {
        if(buffer[i] == 0) {
            break;
        }
        buffer[i] += 1;
    }
}

TEST(inout_c_str, out_c_str_no_change_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_no_change<C>(out_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("test"s, 32) == ""sv);
    CT_EXPECT_TRUE(call(L"test"s, 32) == L""sv);
    CT_EXPECT_TRUE(call(U"test"s, 32) == U""sv);
    CT_EXPECT_TRUE(call(u"test"s, 32) == u""sv);
    CT_EXPECT_TRUE(call(u8"test"s, 32) == u8""sv);
}

TEST(inout_c_str, out_c_str_no_change) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_no_change<C>(out_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("test"s) == ""sv);
    CT_EXPECT_TRUE(call(L"test"s) == L""sv);
    CT_EXPECT_TRUE(call(U"test"s) == U""sv);
    CT_EXPECT_TRUE(call(u"test"s) == u""sv);
    CT_EXPECT_TRUE(call(u8"test"s) == u8""sv);
}

TEST(inout_c_str, inout_c_str_no_change) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_no_change<C>(inout_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("test"s) == "test"sv);
    CT_EXPECT_TRUE(call(L"test"s) == L"test"sv);
    CT_EXPECT_TRUE(call(U"test"s) == U"test"sv);
    CT_EXPECT_TRUE(call(u"test"s) == u"test"sv);
    CT_EXPECT_TRUE(call(u8"test"s) == u8"test"sv);
}

TEST(inout_c_str, out_c_str_c_fn_abc_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_abc<C>(out_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("test"s, 6) == "abcdef"sv);
    CT_EXPECT_TRUE(call(L"test"s, 6) == L"abcdef"sv);
    CT_EXPECT_TRUE(call(U"test"s, 6) == U"abcdef"sv);
    CT_EXPECT_TRUE(call(u"test"s, 6) == u"abcdef"sv);
    CT_EXPECT_TRUE(call(u8"test"s, 6) == u8"abcdef"sv);
}

TEST(inout_c_str, out_c_str_c_fn_abc) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_abc<C>(out_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("test"s) == "abcd"sv);
    CT_EXPECT_TRUE(call(L"test"s) == L"abcd"sv);
    CT_EXPECT_TRUE(call(U"test"s) == U"abcd"sv);
    CT_EXPECT_TRUE(call(u"test"s) == u"abcd"sv);
    CT_EXPECT_TRUE(call(u8"test"s) == u8"abcd"sv);
}

TEST(inout_c_str, inout_c_str_c_fn_abc) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_abc<C>(inout_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("test"s) == "abcd"sv);
    CT_EXPECT_TRUE(call(L"test"s) == L"abcd"sv);
    CT_EXPECT_TRUE(call(U"test"s) == U"abcd"sv);
    CT_EXPECT_TRUE(call(u"test"s) == u"abcd"sv);
    CT_EXPECT_TRUE(call(u8"test"s) == u8"abcd"sv);
}

TEST(inout_c_str, out_c_str_c_fn_plus1_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_plus1<C>(out_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("abcd"s, 4) == ""sv);
    CT_EXPECT_TRUE(call(L"abcd"s, 4) == L""sv);
    CT_EXPECT_TRUE(call(U"abcd"s, 4) == U""sv);
    CT_EXPECT_TRUE(call(u"abcd"s, 4) == u""sv);
    CT_EXPECT_TRUE(call(u8"abcd"s, 4) == u8""sv);
}

TEST(inout_c_str, out_c_str_c_fn_plus1) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_plus1<C>(out_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("abcd"s) == ""sv);
    CT_EXPECT_TRUE(call(L"abcd"s) == L""sv);
    CT_EXPECT_TRUE(call(U"abcd"s) == U""sv);
    CT_EXPECT_TRUE(call(u"abcd"s) == u""sv);
    CT_EXPECT_TRUE(call(u8"abcd"s) == u8""sv);
}

TEST(inout_c_str, inout_c_str_c_fn_plus1) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_plus1<C>(inout_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_TRUE(call("abcd"s) == "bcde"sv);
    CT_EXPECT_TRUE(call(L"abcd"s) == L"bcde"sv);
    CT_EXPECT_TRUE(call(U"abcd"s) == U"bcde"sv);
    CT_EXPECT_TRUE(call(u"abcd"s) == u"bcde"sv);
    CT_EXPECT_TRUE(call(u8"abcd"s) == u8"bcde"sv);
}
