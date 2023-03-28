#include <cadapt/c_str_view.hpp>

#include "test.hpp"

#include <concepts>

using namespace std::literals;
using namespace cadapt;

TEST(c_str_view_test, static_members) {
    static_assert(std::derived_from<c_str_view, std::string_view>);
    static_assert(std::derived_from<wc_str_view, std::wstring_view>);
    static_assert(std::derived_from<u32c_str_view, std::u32string_view>);
    static_assert(std::derived_from<u16c_str_view, std::u16string_view>);
    static_assert(std::derived_from<u8c_str_view, std::u8string_view>);

    struct traits: std::char_traits<char> {};
    static_assert(std::derived_from<basic_c_str_view<char, traits>, std::basic_string_view<char, traits>>);


}
