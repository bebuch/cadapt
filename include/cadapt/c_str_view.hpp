#pragma once

#include <string>
#include <string_view>
#include <stdexcept>


namespace cadapt {


    struct null_terminated_t{};

    static constexpr null_terminated_t null_terminated;

    template <typename C, typename T = std::char_traits<C>>
    struct basic_c_str_view: std::basic_string_view<C, T>{
        static constexpr C const* empty = "";

        static constexpr C const* c_str_or_empty(C const* const c_str) noexcept {
            return c_str == nullptr ? empty : c_str;
        }

        constexpr basic_c_str_view() noexcept
            : std::basic_string_view<C, T>(empty)
            {}

        constexpr basic_c_str_view(basic_c_str_view const&) = default;

        template <typename A>
        constexpr basic_c_str_view(std::basic_string<C, T, A> const& string):
            std::basic_string_view<C, T>(string.c_str(), string.size())
            {}

        constexpr basic_c_str_view(C const* const c_str):
            std::basic_string_view<C, T>(c_str_or_empty(c_str))
            {}

        // efficient but unsave function, expects C != nullptr && c_str[< len] != 0 && c_str[len] == 0
        constexpr basic_c_str_view(null_terminated_t, C const* const c_str, std::size_t const len) noexcept:
            std::basic_string_view<C, T>(c_str, len)
            {}

        constexpr basic_c_str_view(C const* const c_str, std::size_t const len):
           basic_c_str_view(null_terminated, c_str_or_empty(c_str), len)
        {
            if (c_str == nullptr && len > 0) {
                throw std::logic_error("nullptr c_str with length greater zero");
            }

            for (std::size_t i = 0; i < len; ++i) {
                if (c_str[i] == 0) {
                    throw std::logic_error("wrong length for null terminated string");
                }
            }

            if (c_str[len] != 0) {
                throw std::logic_error("not null terminated");
            }
        }

        constexpr basic_c_str_view(std::nullptr_t) = delete;

        constexpr basic_c_str_view& operator=(std::basic_string_view<C, T> const&) noexcept = delete;
        constexpr basic_c_str_view& operator=(basic_c_str_view const&) noexcept = default;

        constexpr C const* c_str() const noexcept {
            return std::basic_string_view<C, T>::data();
        }

        constexpr std::size_t max_size() const noexcept {
            return std::basic_string_view<C, T>::max_size() - 1;
        }

        constexpr void remove_suffix(std::size_t) = delete;

        constexpr void swap(std::basic_string_view<C, T>&) = delete;
        constexpr void swap(basic_c_str_view& view) noexcept {
            std::basic_string_view<C, T>::swap(view);
        }
    };

    template <typename C, typename T, typename A>
    basic_c_str_view(std::basic_string<C, T, A> const&) -> basic_c_str_view<C, T>;


    using c_str_view = basic_c_str_view<char>;
    using u8c_str_view = basic_c_str_view<char8_t>;
    using u16c_str_view = basic_c_str_view<char16_t>;
    using u32c_str_view = basic_c_str_view<char32_t>;
    using wc_str_view = basic_c_str_view<wchar_t>;


    namespace literals {


        constexpr c_str_view operator ""_sv(char const* const str, std::size_t const len) noexcept {
            return c_str_view(null_terminated, str, len);
        }

        constexpr u8c_str_view operator ""_sv(char8_t const* const str, std::size_t const len) noexcept {
            return u8c_str_view(null_terminated, str, len);
        }

        constexpr u16c_str_view operator ""_sv(char16_t const* const str, std::size_t const len) noexcept {
            return u16c_str_view(null_terminated, str, len);
        }

        constexpr u32c_str_view operator ""_sv(char32_t const* const str, std::size_t const len) noexcept {
            return u32c_str_view(null_terminated, str, len);
        }

        constexpr wc_str_view operator ""_sv(wchar_t const* const str, std::size_t const len) noexcept {
            return wc_str_view(null_terminated, str, len);
        }


    }


}


template <typename C, typename T>
inline constexpr bool std::ranges::enable_borrowed_range<cadapt::basic_c_str_view<C, T>> = true;

template <typename C, typename T>
inline constexpr bool std::ranges::enable_view<cadapt::basic_c_str_view<C, T>> = true;

