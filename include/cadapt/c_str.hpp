#pragma once

#include "c_str_view.hpp"

#if __has_include(<QString>)
#include <QString>
#endif


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
    [[nodiscard]] constexpr C const* unverified_c_str(C* const c_str) noexcept {
        return c_str;
    }

    template <typename C, typename T>
    [[nodiscard]] constexpr C const* unverified_c_str(basic_c_str_view<C, T> const view) {
        return view.c_str();
    }

    template <typename C, typename T, typename A>
    [[nodiscard]] constexpr C const* unverified_c_str(std::basic_string<C, T, A> const& string) {
        return string.c_str();
    }

    template <typename C, typename T, typename A = std::allocator<C>>
    [[nodiscard]] constexpr c_str_t<C, T, A> unverified_c_str(std::basic_string_view<C, T> const view) {
        return std::basic_string<C, T, A>(view);
    }

#if __has_include(<QString>)
    template <typename C = char>
    requires same_as_one_of<C, char, wchar_t, char16_t, char32_t>
    [[nodiscard]] constexpr c_str_t<C> unverified_c_str(QString const& string) {
        if constexpr(std::same_as<C, char>){
            return string.toStdString();
        } else if constexpr(std::same_as<C, wchar_t>){
            return string.toStdWString();
        } else if constexpr(std::same_as<C, char16_t>){
            return string.toStdU16String();
        } else if constexpr(std::same_as<C, char32_t>){
            return string.toStdU32String();
        }
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
        return unverified_c_str(view);
    }

    template <typename C, typename T, typename A>
    [[nodiscard]] constexpr C const* c_str(std::basic_string<C, T, A> const& string) {
        verify_c_str_data(string.c_str(), string.size());
        return unverified_c_str(string);
    }

    template <typename C, typename T, typename A = std::allocator<C>>
    [[nodiscard]] constexpr c_str_t<C, T, A> c_str(std::basic_string_view<C, T> const view) {
        verify_c_str_data(view.data(), view.size());
        return unverified_c_str(view);
    }

#if __has_include(<QString>)
    template <typename C = char>
    requires same_as_one_of<C, char, wchar_t, char16_t, char32_t>
    [[nodiscard]] constexpr c_str_t<C> c_str(QString const& string) {
        verify_c_str_data(string.data(), string.size());
        return unverified_c_str<C>(string);
    }
#endif


}
