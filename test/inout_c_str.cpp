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
    bool set_default = false;
    for (std::size_t i = 0; i < buffer_size; ++i) {
        if (buffer[i] == 0) {
            set_default = true;
        }

        if (set_default) {
            buffer[i] = static_cast<C>('1');
        } else {
            buffer[i] = static_cast<C>('a' + i);
        }
    }
}

template <typename C>
static constexpr void c_fn_plus1(C* const buffer, std::size_t const buffer_size) {
    bool set_default = false;
    for (std::size_t i = 0; i < buffer_size; ++i) {
        if (buffer[i] == 0) {
            set_default = true;
        }

        if (set_default) {
            buffer[i] = static_cast<C>('1');
        } else {
            buffer[i] += 1;
        }
    }
}

TEST(inout_c_str, out_c_str_no_change_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_no_change<C>(out_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("test"s, 6), ""sv);
    CT_EXPECT_EQ(call(L"test"s, 6), L""sv);
    CT_EXPECT_EQ(call(U"test"s, 6), U""sv);
    CT_EXPECT_EQ(call(u"test"s, 6), u""sv);
    CT_EXPECT_EQ(call(u8"test"s, 6), u8""sv);
}

TEST(inout_c_str, out_c_str_no_change) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_no_change<C>(out_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("test"s), ""sv);
    CT_EXPECT_EQ(call(L"test"s), L""sv);
    CT_EXPECT_EQ(call(U"test"s), U""sv);
    CT_EXPECT_EQ(call(u"test"s), u""sv);
    CT_EXPECT_EQ(call(u8"test"s), u8""sv);
}

TEST(inout_c_str, inout_c_str_no_change_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_no_change<C>(inout_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("test"s, 6), "test"sv);
    CT_EXPECT_EQ(call(L"test"s, 6), L"test"sv);
    CT_EXPECT_EQ(call(U"test"s, 6), U"test"sv);
    CT_EXPECT_EQ(call(u"test"s, 6), u"test"sv);
    CT_EXPECT_EQ(call(u8"test"s, 6), u8"test"sv);
}

TEST(inout_c_str, inout_c_str_no_change) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_no_change<C>(inout_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("test"s), "test"sv);
    CT_EXPECT_EQ(call(L"test"s), L"test"sv);
    CT_EXPECT_EQ(call(U"test"s), U"test"sv);
    CT_EXPECT_EQ(call(u"test"s), u"test"sv);
    CT_EXPECT_EQ(call(u8"test"s), u8"test"sv);
}

TEST(inout_c_str, out_c_str_c_fn_abc_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_abc<C>(out_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    // TODO: GCC 12.1 bug prohibits CT
    EXPECT_EQ(call("test"s, 6), "111111"sv);
    CT_EXPECT_EQ(call(L"test"s, 6), L"111111"sv);
    CT_EXPECT_EQ(call(U"test"s, 6), U"111111"sv);
    EXPECT_EQ(call(u"test"s, 6), u"111111"sv);
    EXPECT_EQ(call(u8"test"s, 6), u8"111111"sv);
}

TEST(inout_c_str, out_c_str_c_fn_abc) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_abc<C>(out_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("test"s), "1111"sv);
    CT_EXPECT_EQ(call(L"test"s), L"1111"sv);
    CT_EXPECT_EQ(call(U"test"s), U"1111"sv);
    CT_EXPECT_EQ(call(u"test"s), u"1111"sv);
    CT_EXPECT_EQ(call(u8"test"s), u8"1111"sv);
}

TEST(inout_c_str, inout_c_str_c_fn_abc_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_abc<C>(inout_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    // TODO: GCC 12.1 bug prohibits CT
    EXPECT_EQ(call("test"s, 6), "abcd11"sv);
    CT_EXPECT_EQ(call(L"test"s, 6), L"abcd11"sv);
    CT_EXPECT_EQ(call(U"test"s, 6), U"abcd11"sv);
    EXPECT_EQ(call(u"test"s, 6), u"abcd11"sv);
    EXPECT_EQ(call(u8"test"s, 6), u8"abcd11"sv);
}

TEST(inout_c_str, inout_c_str_c_fn_abc) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_abc<C>(inout_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("test"s), "abcd"sv);
    CT_EXPECT_EQ(call(L"test"s), L"abcd"sv);
    CT_EXPECT_EQ(call(U"test"s), U"abcd"sv);
    CT_EXPECT_EQ(call(u"test"s), u"abcd"sv);
    CT_EXPECT_EQ(call(u8"test"s), u8"abcd"sv);
}

TEST(inout_c_str, out_c_str_c_fn_plus1_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_plus1<C>(out_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    // TODO: GCC 12.1 bug prohibits CT
    EXPECT_EQ(call("abcd"s, 6), "111111"sv);
    CT_EXPECT_EQ(call(L"abcd"s, 6), L"111111"sv);
    CT_EXPECT_EQ(call(U"abcd"s, 6), U"111111"sv);
    EXPECT_EQ(call(u"abcd"s, 6), u"111111"sv);
    EXPECT_EQ(call(u8"abcd"s, 6), u8"111111"sv);
}

TEST(inout_c_str, out_c_str_c_fn_plus1) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_plus1<C>(out_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("abcd"s), "1111"sv);
    CT_EXPECT_EQ(call(L"abcd"s), L"1111"sv);
    CT_EXPECT_EQ(call(U"abcd"s), U"1111"sv);
    CT_EXPECT_EQ(call(u"abcd"s), u"1111"sv);
    CT_EXPECT_EQ(call(u8"abcd"s), u8"1111"sv);
}

TEST(inout_c_str, inout_c_str_c_fn_plus1_with_size) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer, std::size_t const buffer_size) {
        c_fn_plus1<C>(inout_c_str(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    // TODO: GCC 12.1 bug prohibits CT
    EXPECT_EQ(call("abcd"s, 6), "bcde11"sv);
    CT_EXPECT_EQ(call(L"abcd"s, 6), L"bcde11"sv);
    CT_EXPECT_EQ(call(U"abcd"s, 6), U"bcde11"sv);
    EXPECT_EQ(call(u"abcd"s, 6), u"bcde11"sv);
    EXPECT_EQ(call(u8"abcd"s, 6), u8"bcde11"sv);
}

TEST(inout_c_str, inout_c_str_c_fn_plus1) {
    static constexpr auto call = []<typename C>(std::basic_string<C>&& buffer) {
        c_fn_plus1<C>(inout_c_str(buffer), buffer.size());
        return std::move(buffer);
    };

    CT_EXPECT_EQ(call("abcd"s), "bcde"sv);
    CT_EXPECT_EQ(call(L"abcd"s), L"bcde"sv);
    CT_EXPECT_EQ(call(U"abcd"s), U"bcde"sv);
    CT_EXPECT_EQ(call(u"abcd"s), u"bcde"sv);
    CT_EXPECT_EQ(call(u8"abcd"s), u8"bcde"sv);
}

#if __has_include(<QString>)
using std::type_identity;

TEST(inout_c_str, qt_out_c_str_no_change_with_size) {
    static constexpr auto call = []<typename C>(type_identity<C>, QString&& buffer, std::size_t const buffer_size) {
        c_fn_no_change<C>(out_c_str<C>(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    EXPECT_EQ(call(type_identity<char>{}, QString("test"), 6), QString(""));
    EXPECT_EQ(call(type_identity<wchar_t>{}, QString("test"), 6), QString(""));
    EXPECT_EQ(call(type_identity<char32_t>{}, QString("test"), 6), QString(""));
    EXPECT_EQ(call(type_identity<char16_t>{}, QString("test"), 6), QString(""));
}

TEST(inout_c_str, qt_inout_c_str_no_change_with_size) {
    static constexpr auto call = []<typename C>(type_identity<C>, QString&& buffer, std::size_t const buffer_size) {
        c_fn_no_change<C>(inout_c_str<C>(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    EXPECT_EQ(call(type_identity<char>{}, QString("test"), 6), QString("test"));
    EXPECT_EQ(call(type_identity<wchar_t>{}, QString("test"), 6), QString("test"));
    EXPECT_EQ(call(type_identity<char32_t>{}, QString("test"), 6), QString("test"));
    EXPECT_EQ(call(type_identity<char16_t>{}, QString("test"), 6), QString("test"));
}

TEST(inout_c_str, qt_out_c_str_c_fn_abc_with_size) {
    static constexpr auto call = []<typename C>(type_identity<C>, QString&& buffer, std::size_t const buffer_size) {
        c_fn_abc<C>(out_c_str<C>(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    EXPECT_EQ(call(type_identity<char>{}, QString("test"), 6), QString("111111"));
    EXPECT_EQ(call(type_identity<wchar_t>{}, QString("test"), 6), QString("111111"));
    EXPECT_EQ(call(type_identity<char32_t>{}, QString("test"), 6), QString("111111"));
    EXPECT_EQ(call(type_identity<char16_t>{}, QString("test"), 6), QString("111111"));
}

TEST(inout_c_str, qt_inout_c_str_c_fn_abc_with_size) {
    static constexpr auto call = []<typename C>(type_identity<C>, QString&& buffer, std::size_t const buffer_size) {
        c_fn_abc<C>(inout_c_str<C>(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    EXPECT_EQ(call(type_identity<char>{}, QString("test"), 6), QString("abcd11"));
    EXPECT_EQ(call(type_identity<wchar_t>{}, QString("test"), 6), QString("abcd11"));
    EXPECT_EQ(call(type_identity<char32_t>{}, QString("test"), 6), QString("abcd11"));
    EXPECT_EQ(call(type_identity<char16_t>{}, QString("test"), 6), QString("abcd11"));
}

TEST(inout_c_str, qt_out_c_str_c_fn_plus1_with_size) {
    static constexpr auto call = []<typename C>(type_identity<C>, QString&& buffer, std::size_t const buffer_size) {
        c_fn_plus1<C>(out_c_str<C>(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    EXPECT_EQ(call(type_identity<char>{}, QString("abcd"), 6), QString("111111"));
    EXPECT_EQ(call(type_identity<wchar_t>{}, QString("abcd"), 6), QString("111111"));
    EXPECT_EQ(call(type_identity<char32_t>{}, QString("abcd"), 6), QString("111111"));
    EXPECT_EQ(call(type_identity<char16_t>{}, QString("abcd"), 6), QString("111111"));
}


TEST(inout_c_str, qt_inout_c_str_c_fn_plus1_with_size) {
    static constexpr auto call = []<typename C>(type_identity<C>, QString&& buffer, std::size_t const buffer_size) {
        c_fn_plus1<C>(inout_c_str<C>(buffer, buffer_size), buffer_size);
        return std::move(buffer);
    };

    EXPECT_EQ(call(type_identity<char>{}, QString("abcd"), 6), QString("bcde11"));
    EXPECT_EQ(call(type_identity<wchar_t>{}, QString("abcd"), 6), QString("bcde11"));
    EXPECT_EQ(call(type_identity<char32_t>{}, QString("abcd"), 6), QString("bcde11"));
    EXPECT_EQ(call(type_identity<char16_t>{}, QString("abcd"), 6), QString("bcde11"));
}
#endif
