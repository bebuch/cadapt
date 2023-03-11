#pragma once

#ifdef __cpp_lib_modules
import std;
#else // Support Workaround pre C++23
#include <string>
#include <algorithm>
#endif


namespace cadapt {


    // NOTE: Template default arguments for simpler explicit instantiation

    /// Does resize the `string` to `count` and sets its content to an empty C string. The characters at positions
    /// `0` and `count` are null terminators after the function call. All other characters have undefined values
    /// after the function call.
    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    constexpr void resize_for_out_ptr(
        std::basic_string<C, T, A>& string,
        typename std::basic_string<C, T, A>::size_type const count
    ) {
    #ifdef __cpp_lib_string_resize_and_overwrite
        string.resize_and_overwrite(count, [count](char*, std::size_t) {
            return count;
        });
    #else // Support Workaround pre C++23
        string.resize(count);
    #endif

        string[0] = '\0';
    }


    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    class out_c_str_t{
    public:
        using value_type = std::basic_string<C, T, A>;
        using size_type = typename value_type::size_type;

        out_c_str_t(value_type& target)
            : target_(target)
            {}

        out_c_str_t(value_type& target, size_type const max_target_length)
            : out_c_str_t(target)
        {
            resize_for_out_ptr(target, max_target_length);
        }

        out_c_str_t(out_c_str_t const&) = delete;

        ~out_c_str_t() {
            target_.resize(
                static_cast<size_type>(std::find(target_.cbegin(), target_.cend(), '\0') - target_.cbegin()));
        }

        operator C*()&& noexcept {
            return const_cast<C*>(target_.c_str());
        }

        operator C*()const& noexcept {
            static constexpr auto always_false = sizeof(C) == 0;
            static_assert(always_false, "You have created a variable of type 'out_c_str_t' which can easily be used "
                "incorrectly! Please use 'out_c_str(your_string)' as argument in your C function call without creating "
                "a variable. The reason for this is that 'your_string' could have an incorrect size and contain memory "
                "garbage between your C function call and the destruction of the 'out_c_str_t' object. If you are sure "
                "you know what you are doing, you can use 'std::move' with your C variable 'out_c_str_t'.");
            return nullptr;
        }

    private:
        value_type& target_;
    };


    // NOTE: You can take the address of an function, but not of an constructor

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    out_c_str_t<C, T, A> out_c_str(std::basic_string<C, T, A>& target) {
        return {target};
    }

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    out_c_str_t<C, T, A> out_c_str(
        std::basic_string<C, T, A>& target,
        typename std::basic_string<C, T, A>::size_type const max_target_length
    ) {
        return {target, max_target_length};
    }


}
