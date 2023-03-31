#pragma once

#include <concepts>
#include <execution>
#include <functional>
#include <type_traits>


namespace cadapt {


    template <typename Ref, typename ... Tests>
    concept same_as_one_of = (std::same_as<Ref, Tests> || ...);

    // Warning: Modifications are nevertheless forbidden and lead to undefined behavior!
    template <typename T>
    constexpr decltype(auto) non_const(T&& ref) noexcept {
        using raw_type = std::remove_reference_t<T>;
        if constexpr(std::is_pointer_v<raw_type>) {
            return const_cast<std::remove_const_t<std::remove_pointer_t<raw_type>>*>(ref);
        } else if constexpr(std::is_lvalue_reference_v<T&&>) {
            return const_cast<std::add_lvalue_reference_t<std::remove_const_t<raw_type>>>(ref);
        } else {
            return const_cast<std::add_rvalue_reference_t<std::remove_const_t<raw_type>>>(ref);
        }
    }


}
