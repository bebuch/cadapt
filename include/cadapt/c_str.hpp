#pragma once

#include "c_str_view.hpp"


namespace cadapt {


    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    class c_str_t {
    public:
        [[nodiscard]] c_str_t() = delete;

        [[nodiscard]] constexpr c_str_t(std::basic_string<C, T, A>&& value)
            : value_(std::move(value))
            {}

        c_str_t(c_str_t const&) = delete;

        [[nodiscard]] constexpr operator C const*() const noexcept {
            return value_.c_str();
        }

    private:
        std::basic_string<C, T, A> const value_;
    };

    template <typename C>
    [[nodiscard]] constexpr C const* c_str(null_term_t, C* const c_str) noexcept {
        return c_str;
    }

    template <typename C, typename T>
    [[nodiscard]] constexpr C const* c_str(null_term_t, basic_c_str_view<C, T> const view) {
        return view.c_str();
    }

    template <typename C, typename T, typename A>
    [[nodiscard]] constexpr C const* c_str(null_term_t, std::basic_string<C, T, A> const& string) {
        return string.c_str();
    }

    template <typename C, typename T, typename A = std::allocator<C>>
    [[nodiscard]] constexpr c_str_t<C, T, A> c_str(null_term_t, std::basic_string_view<C, T> const view) {
        return std::basic_string<C, T, A>(view);
    }

#if __has_include(<QString>)
    template <typename C = char>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    [[nodiscard]] constexpr c_str_t<C> c_str(null_term_t, QString const& string) {
        return qt_to_std<C>(string);
    }
#endif

    template <typename C, std::size_t N>
    requires (N > 0)
    [[nodiscard]] constexpr C const* c_str(C(&c_str)[N]) {
        verify_c_str_data(c_str, N - 1);
        verify_c_str_terminator(c_str, N - 1);
        return c_str;
    }

    template <typename C, typename T>
    [[nodiscard]] constexpr C const* c_str(basic_c_str_view<C, T> const view) {
        verify_c_str_data(view.c_str(), view.size());
        return c_str(null_term, view);
    }

    template <typename C, typename T, typename A>
    [[nodiscard]] constexpr C const* c_str(std::basic_string<C, T, A> const& string) {
        verify_c_str_data(string.c_str(), string.size());
        return c_str(null_term, string);
    }

    template <typename C, typename T, typename A = std::allocator<C>>
    [[nodiscard]] constexpr c_str_t<C, T, A> c_str(std::basic_string_view<C, T> const view) {
        verify_c_str_data(view.data(), view.size());
        return c_str(null_term, view);
    }

#if __has_include(<QString>)
    template <typename C = char>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    [[nodiscard]] constexpr c_str_t<C> c_str(QString const& string) {
        verify_c_str_data(string.data(), string.size());
        return c_str<C>(null_term, string);
    }
#endif


}
