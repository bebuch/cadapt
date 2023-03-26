#pragma once

#ifdef __cpp_lib_modules
import std;
#else // Support Workaround pre C++23
#include <string>
#include <algorithm>
#endif


namespace cadapt {


    // NOTE: Template default arguments for simpler explicit instantiation

    template <typename Container>
    constexpr void resize_uninitialized(
        Container& container,
        typename std::basic_string<C, T, A>::size_type const count
    ) {
    #ifdef __cpp_lib_string_resize_and_overwrite
        string.resize_and_overwrite(count, [count](T*, std::size_t) {
            return count;
        });
    #else // Support Workaround pre C++23
        string.resize(count);
    #endif
    }


    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    class out_buffer_t{
    public:
        using value_type = std::basic_string<C, T, A>;
        using size_type = typename value_type::size_type;

        out_buffer_t(value_type& target)
            : target_(target)
            {}

        out_buffer_t(value_type& target, size_type const max_target_length)
            : out_buffer_t(target)
        {
            resize_for_out_ptr(target, max_target_length);
        }

        out_buffer_t(out_buffer_t const&) = delete;

        ~out_buffer_t() {
            target_.resize(
                static_cast<size_type>(std::find(target_.cbegin(), target_.cend(), '\0') - target_.cbegin()));
        }

        operator C*()&& noexcept {
            return target_.data();
        }

        operator C*()const& noexcept {
            static constexpr auto always_false = sizeof(C) == 0;
            static_assert(always_false, "You have created a variable of type 'out_buffer_t' which can easily be used "
                "incorrectly! Please use 'out_buffer(your_container)' as argument in your C function call without creating "
                "a variable. The reason for this is that 'your_container' could have an incorrect size and contain memory "
                "garbage between your C function call and the destruction of the 'out_buffer_t' object. If you are sure "
                "you know what you are doing, you can use 'std::move' with your C variable 'out_buffer_t'.");
            return nullptr;
        }

    private:
        value_type& target_;
    };


    // NOTE: You can take the address of an function, but not of an constructor

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    out_buffer_t<C, T, A> out_buffer(std::basic_string<C, T, A>& target) {
        return {target};
    }

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    out_buffer_t<C, T, A> out_buffer(
        std::basic_string<C, T, A>& target,
        typename std::basic_string<C, T, A>::size_type const max_target_length
    ) {
        return {target, max_target_length};
    }


}
