#pragma once

#include <concepts>


namespace cadapt {


    template <typename Ref, typename ... Tests>
    concept same_as_one_of = (std::same_as<Ref, Tests> || ...);

    // Warning: Modifications are nevertheless forbidden and lead to undefined behavior!
    template <typename T>
    T* without_const(T const* ptr) noexcept {
        return const_cast<T*>(ptr);
    }


}
