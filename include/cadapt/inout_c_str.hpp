#pragma once

#include "utility.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>


namespace cadapt {


    /// Does resize the `string` to `count` and sets its content to an empty C string. The character at position
    /// `count` is a null terminators after the function call. All other characters have undefined values
    /// after the function call.
    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    constexpr void resize_for_out_c_str(std::basic_string<C, T, A>& string, std::size_t const count) {
        string.clear(); // make sure the following resize don't care about the old content

    #ifdef __cpp_lib_string_resize_and_overwrite
        string.resize_and_overwrite(count, [count](C*, std::size_t) {
            return count;
        });
    #else // Support Workaround pre C++23
        string.resize(count);
    #endif
    }

    /// Does resize the `string` to `count` without changing its c_str(). The new size must be equal to or greater then
    /// the previous size.
    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    constexpr void enlarge_for_inout_c_str(std::basic_string<C, T, A>& string, std::size_t const count) {
        auto const previous_size = string.size();

        if (count < previous_size) {
            throw std::logic_error("inout_ptr enlarge to size that is smaller then previous size");
        }

    #ifdef __cpp_lib_string_resize_and_overwrite
        string.resize_and_overwrite(count, [count](C*, std::size_t) {
            return count;
        });
    #else // Support Workaround pre C++23
        string.resize(count);
    #endif

        string[previous_size] = 0;
    }

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    class inout_c_str_t{
    public:
        [[nodiscard]] constexpr inout_c_str_t(std::basic_string<C, T, A>& target) noexcept
            : target_(target)
            {}

        inout_c_str_t(inout_c_str_t const&) = delete;

        constexpr ~inout_c_str_t() {
            target_.resize(static_cast<std::size_t>(
                std::find(target_.cbegin(), target_.cend(), 0) - target_.cbegin()));
        }

        [[nodiscard]] constexpr operator C*()&& noexcept {
            return target_.data();
        }

        [[nodiscard]] constexpr operator C*()const& noexcept {
            constexpr auto always_false = sizeof(C) == 0;
            static_assert(always_false, "You have created a variable of type 'inout_c_str_t' which can easily be "
                "used incorrectly! Please use 'out_c_str(string_variable)' as argument in your C function call without "
                "creating a variable. The reason for this is that 'string_variable' could have an incorrect size and "
                "contain memory garbage between your C function call and the destruction of the 'inout_c_str_t' "
                "object. If you are sure you know what you are doing, you can use 'std::move' on your "
                "'inout_c_str_t' variable.");
            return nullptr;
        }

    private:
        std::basic_string<C, T, A>& target_;
    };


    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    [[nodiscard]] constexpr auto inout_c_str(std::basic_string<C, T, A>& target) noexcept
    -> inout_c_str_t<C, T, A> {
        return target;
    }

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    [[nodiscard]] constexpr auto inout_c_str(std::basic_string<C, T, A>& target, std::size_t const max_target_length)
    -> inout_c_str_t<C, T, A> {
        enlarge_for_inout_c_str(target, max_target_length);
        return inout_c_str(target);
    }

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    [[nodiscard]] constexpr auto out_c_str(std::basic_string<C, T, A>& target)
    -> inout_c_str_t<C, T, A> {
        target[0] = 0;
        return target;
    }

    template <typename C, typename T = std::char_traits<C>, typename A = std::allocator<C>>
    [[nodiscard]] constexpr auto out_c_str(std::basic_string<C, T, A>& target, std::size_t const max_target_length)
    -> inout_c_str_t<C, T, A> {
        resize_for_out_c_str(target, max_target_length);
        return out_c_str(target);
    }


#if __has_include(<QString>)
    template <typename C>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    class inout_c_str_qt_t{
    public:
        [[nodiscard]] constexpr inout_c_str_qt_t(QString& user_target, std::basic_string<C>&& c_str_target) noexcept
            : user_target_(user_target)
            , c_str_target_(std::move(c_str_target))
            {}

        inout_c_str_qt_t(inout_c_str_qt_t const&) = delete;

        constexpr ~inout_c_str_qt_t() {
            c_str_target_.resize(static_cast<std::size_t>(
                std::find(c_str_target_.cbegin(), c_str_target_.cend(), 0) - c_str_target_.cbegin()));
            user_target_ = std_to_qt(c_str_target_);
        }

        [[nodiscard]] constexpr operator C*()&& noexcept {
            return c_str_target_.data();
        }

        [[nodiscard]] constexpr operator C*()const& noexcept {
            constexpr auto always_false = sizeof(C) == 0;
            static_assert(always_false, "You have created a variable of type 'inout_c_str_qt_t' which can easily be "
                "used incorrectly! Please use 'out_c_str(string_variable)' as argument in your C function call without "
                "creating a variable. The reason for this is that 'string_variable' could have an incorrect size and "
                "contain memory garbage between your C function call and the destruction of the 'inout_c_str_qt_t' "
                "object. If you are sure you know what you are doing, you can use 'std::move' on your "
                "'inout_c_str_qt_t' variable.");
            return nullptr;
        }

    private:
        QString& user_target_;
        std::basic_string<C> c_str_target_;
    };

    template <typename C = char>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    [[nodiscard]] constexpr auto inout_c_str(QString& target, std::size_t const max_target_length) noexcept
    -> inout_c_str_qt_t<C> {
        auto c_str_target = qt_to_std<C>(target);
        enlarge_for_inout_c_str(c_str_target, max_target_length);
        return {target, std::move(c_str_target)};
    }

    template <typename C = char>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    [[nodiscard]] constexpr auto out_c_str(QString& target, std::size_t const max_target_length)
    -> inout_c_str_qt_t<C> {
        auto c_str_target = qt_to_std<C>(target);
        resize_for_out_c_str(c_str_target, max_target_length);
        c_str_target[0] = 0;
        return {target, std::move(c_str_target)};
    }
#endif


}
