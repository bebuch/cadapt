#include <cadapt/inout_c_str.hpp>

#include "test.hpp"

using namespace std::literals;
using namespace cadapt;


TEST(inout_c_str, resize_for_out_c_str) {
    static constexpr auto test = []<typename T>(std::basic_string<T> target) {
        resize_for_out_c_str(target, 4);
        ASSERT_EQ(target.size(), 4);
        EXPECT_EQ(target[0], 0);
        resize_for_out_c_str(target, 512);
        ASSERT_EQ(target.size(), 512);
        EXPECT_EQ(target[0], 0);
    };

    test(std::string{});
    test(std::wstring{});
    test(std::u32string{});
    test(std::u16string{});
    test(std::u8string{});

    static constexpr auto ct_resize = []<typename T>(std::basic_string<T>&& target, std::size_t count) {
        resize_for_out_c_str(target, count);
        return std::make_pair(std::basic_string_view(target.c_str()), target.size());
    };

    static_assert(ct_resize("test"s, 32) == std::make_pair(""sv, std::size_t(32)));
    static_assert(ct_resize(L"test"s, 32) == std::make_pair(L""sv, std::size_t(32)));
    static_assert(ct_resize(U"test"s, 32) == std::make_pair(U""sv, std::size_t(32)));
    static_assert(ct_resize(u"test"s, 32) == std::make_pair(u""sv, std::size_t(32)));
    static_assert(ct_resize(u8"test"s, 32) == std::make_pair(u8""sv, std::size_t(32)));
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
