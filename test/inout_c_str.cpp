#include <cadapt/inout_c_str.hpp>

#include "test.hpp"

using namespace std::literals;
using namespace cadapt;


TEST(inout_c_str, resize_for_out_ptr) {
    static constexpr auto test = []<typename T>(std::basic_string<T> target) {
        resize_for_out_ptr(target, 4);
        ASSERT_EQ(target.size(), 4);
        EXPECT_EQ(target[0], 0);
        resize_for_out_ptr(target, 512);
        ASSERT_EQ(target.size(), 512);
        EXPECT_EQ(target[0], 0);
    };

    test(std::string{});
    test(std::wstring{});
    test(std::u32string{});
    test(std::u16string{});
    test(std::u8string{});
}

TEST(inout_c_str, enlarge_for_inout_ptr) {
    static constexpr auto test = []<typename T>(std::basic_string<T> target) {
        auto const original_c_str = std::basic_string<T>(target.c_str());

        enlarge_for_inout_ptr(target, 32);
        ASSERT_EQ(target.size(), 32);
        ASSERT_EQ(original_c_str, target.c_str());
        enlarge_for_inout_ptr(target, 512);
        ASSERT_EQ(target.size(), 512);
        ASSERT_EQ(original_c_str, target.c_str());
        EXPECT_THROW(enlarge_for_inout_ptr(target, 384), std::logic_error);
    };

    test("test"s);
    test(L"test"s);
    test(U"test"s);
    test(u"test"s);
    test(u8"test"s);
}
