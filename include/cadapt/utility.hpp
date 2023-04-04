#pragma once

#include <concepts>
#include <execution>
#include <functional>
#include <type_traits>

#if __has_include(<QString>)
#include <QString>
#endif


namespace cadapt {


    template <typename Ref, typename ... Tests>
    concept same_as_one_of = (std::same_as<Ref, Tests> || ...);

    // Warning: Modifications are nevertheless forbidden and lead to undefined behavior!
    template <typename T>
    constexpr decltype(auto) non_const(T&& ref) noexcept {
        using unref_type = std::remove_reference_t<T>;
        if constexpr(std::is_pointer_v<unref_type>) {
            return const_cast<std::remove_const_t<std::remove_pointer_t<unref_type>>*>(ref);
        } else if constexpr(std::is_lvalue_reference_v<T&&>) {
            // the std::forward is a workaround for a MSVC bug
            using type = std::remove_const_t<unref_type>&;
            return std::forward<type>(const_cast<type>(ref));
        } else {
            // the std::forward is a workaround for a MSVC bug
            using type = std::remove_const_t<unref_type>&&;
            return std::forward<type>(const_cast<type>(ref));
        }
    }

#if __has_include(<QString>)
    template <typename C>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    [[nodiscard]] constexpr std::basic_string<C> qt_to_std(QString const& string) {
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

    template <typename C>
    requires same_as_one_of<C, char, wchar_t, char32_t, char16_t>
    [[nodiscard]] constexpr QString std_to_qt(std::basic_string<C> const& string) {
        if constexpr(std::same_as<C, char>){
            return QString::fromStdString(string);
        } else if constexpr(std::same_as<C, wchar_t>){
            return QString::fromStdWString(string);
        } else if constexpr(std::same_as<C, char16_t>){
            return QString::fromStdU16String(string);
        } else if constexpr(std::same_as<C, char32_t>){
            return QString::fromStdU32String(string);
        }
    }
#endif



}
