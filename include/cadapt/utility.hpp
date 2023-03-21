#pragma once


namespace cadapt {


    // Warning: Modifications are nevertheless forbidden and lead to undefined behavior!
    template <typename T>
    T* without_const(T const* ptr) noexcept {
        return const_cast<T*>(ptr);
    }


}
