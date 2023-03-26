#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <execution>


namespace cadapt {


    template <typename Ref, typename ... Tests>
    concept same_as_one_of = (std::same_as<Ref, Tests> || ...);

    // Warning: Modifications are nevertheless forbidden and lead to undefined behavior!
    template <typename T>
    constexpr T* without_const(T const* ptr) noexcept {
        return const_cast<T*>(ptr);
    }

    template <typename Fn, typename ... T>
    constexpr decltype(auto) unseq_invoke(Fn&& fn, T&& ... v) {
        if (std::is_constant_evaluated()) {
            return std::invoke(std::forward<Fn>(fn), std::forward<T>(v) ...);
        } else {
            return std::invoke(std::forward<Fn>(fn), std::execution::unseq, std::forward<T>(v) ...);
        }
    }


}
