#include <cadapt/inout_c_str.hpp>

#include "test.hpp"

using namespace std::literals;
using namespace cadapt;


TEST(inout_c_str, resize_for_out_ptr) {
    static constexpr auto test = []<typename T>(std::basic_string<T> target) {
        resize_for_out_ptr(target, 4);
        ASSERT_EQ(target.size(), 4);
        ASSERT_EQ(target[0], 0);
        resize_for_out_ptr(target, 512);
        ASSERT_EQ(target.size(), 512);
        ASSERT_EQ(target[0], 0);
    };

    test(std::string{});
    test(std::wstring{});
    test(std::u32string{});
    test(std::u16string{});
    test(std::u8string{});
}
