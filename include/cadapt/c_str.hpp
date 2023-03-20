#pragma once

#include "c_str_view.hpp"


namespace cadapt {


    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    class c_str_t {
    public:
        c_str_t() = delete;

        constexpr c_str_t(std::basic_string<C, T, A>&& value) noexcept
            : value_(std::move(value))
            {}

        c_str_t(c_str_t const&) = delete;

        constexpr operator C const*() const noexcept {
            return value_.c_str();
        }

    private:
        std::basic_string<C, T, A> const value_;
    };

    template <typename C>
    C const* c_str(C const* c_str) noexcept {
        return c_str;
    }

    template <typename C, typename T>
    C const* c_str(basic_c_str_view<C, T> const view) noexcept {
        return view.c_str();
    }

    template <typename C, typename T, typename A>
    C const* c_str(std::basic_string<C, T, A> const& string) noexcept {
        return string.c_str();
    }

    template <typename C, typename T, typename A = std::allocator<C>>
    c_str_t<C, T, A> c_str(std::basic_string_view<C, T> const view) {
        return c_str_t<C, T, A>(std::basic_string<C, T, A>(view));
    }


}
