# CAdapt

CAdapt is a C++ header only library for simplified and safe use of C functions. The minimum requirement is C++20.

The library has wrapper support for `QString` if Qt6 is found on the system.

**The library is still in an early stage!**

The use of C++23 improves performance at runtime for `out_c_str()` and `inout_c_str()`. Buffers are left uninitialized when enlarging. This is realized by using C++23 `resize_and_overwrite()` instead of the older `resize()`.

## Usage

TODO: CMake.

## Documentation

All tools are in the `namespace cadapt`. User-defined-literals are in the `inline namespace cadapt::literals`.

Currently, the library includes the following tools:

- `basic_c_str_view` is a `std::basic_string_view` which is guaranteed to be a completely valid null-terminated C-string
    - `""_sv` const-evaluated user-defined-literal
    - `c_str_view` (`"text"_sv`) based on `std::string_view`
    - `wc_str_view` (`L"text"_sv`) based on `std::wstring_view`
    - `u32c_str_view` (`U"text"_sv`) based on `std::u32string_view`
    - `u16c_str_view` (`u"text"_sv`) based on `std::u16string_view`
    - `u8c_str_view` (`u8"text"_sv`) based on `std::u8string_view`
- `c_str` to get a C-string from an object of type
    - `basic_c_str_view`
    - `std::basic_string_view`
    - `std::basic_string`
    - `QString`
- `out_c_str` and `inout_c_str` to pass a C-string output argument to an object of type
    - `std::basic_string`
    - `QString`

The `QString` versions use internally conversions from and to `std::basic_string` and are therefore slower and partly have a slightly different interface. Use the normal `std::basic_string` versions by default. If data is available as `QString`, then use the wrapper versions, as they are safer and more efficient than a manual conversion.

### `null_term` and `null_term_t`

```cpp
struct null_term_t{
    explicit null_term_t() = default;
};

inline constexpr null_term_t null_term{};
```

`null_term` is a disambiguation tag that can be passed to some functions to indicate that the following given data is guaranteed to be a valid C-string. **This disables the data verification!**

The corresponding type `null_term_t` is used in function parameter list.

### `c_str_view`

```cpp
template <typename C, typename T = std::char_traits<C>>
struct basic_c_str_view: std::basic_string_view<C, T>
```

A `basic_c_str_view` is a `std::basic_string_view` which can only be constructed from data that is a valid C-string.

A valid C-string has the following properties:

1. It is not a `nullptr`.
1. It is terminated by a null character.
1. It does not contain any null character except for the terminator.

```cpp
using c_str_view = basic_c_str_view<char>;
using wc_str_view = basic_c_str_view<wchar_t>;
using u32c_str_view = basic_c_str_view<char32_t>;
using u16c_str_view = basic_c_str_view<char16_t>;
using u8c_str_view = basic_c_str_view<char8_t>;
```

#### Literals

```cpp
inline namespace literals {
    consteval c_str_view operator""_sv(char const* str, std::size_t len);
    consteval wc_str_view operator""_sv(wchar_t const* str, std::size_t len);
    consteval u32c_str_view operator""_sv(char32_t const* str, std::size_t len);
    consteval u16c_str_view operator""_sv(char16_t const* str, std::size_t len);
    consteval u8c_str_view operator""_sv(char8_t const* str, std::size_t len);
}
```

Whenever possible, use these custom literals to create a `basic_c_str_view`. The operators are guaranteed to be executed at compile time and therefore verify the contents without any runtime overhead.

Example usage:

```cpp
#include <cadapt/c_str_view.hpp>

int main() {
    using namespace cadapt::literals;
    auto const char_view = "text of type cadapt::c_str_view"_sv;
    auto const wchar_view = L"text of type cadapt::wc_str_view"_sv;
    auto const u32char_view = U"text of type cadapt::u32c_str_view"_sv;
    auto const u16char_view = u"text of type cadapt::u16c_str_view"_sv;
    auto const u8char_view = u8"text of type cadapt::u8c_str_view"_sv;
}
```

#### Constructors

```cpp
{
    static constexpr C const empty[1]{};

    [[nodiscard]] static constexpr C const* c_str_or_empty(C const* const c_str) noexcept {
        return c_str == nullptr ? empty : c_str;
    }

    [[nodiscard]] constexpr basic_c_str_view() noexcept
        : std::basic_string_view<C, T>(empty)
        {}

    [[nodiscard]] constexpr basic_c_str_view(std::nullptr_t) noexcept
        : std::basic_string_view<C, T>(empty)
        {}

    template <typename P>
    requires (std::is_pointer_v<P> && std::convertible_to<P, C const*>)
    [[nodiscard]] constexpr basic_c_str_view(P c_str) noexcept:
        std::basic_string_view<C, T>(c_str_or_empty(c_str))
        {}

    [[nodiscard]] constexpr basic_c_str_view(C const* const c_str, std::size_t const len):
        std::basic_string_view<C, T>(c_str_or_empty(c_str), len)
    {
        verify_c_str_ptr(c_str, len);
        verify_c_str_data();
        verify_c_str_terminator();
    }

    // efficient but unsave function, expects C != nullptr && c_str[< len] != 0 && c_str[len] == 0
    [[nodiscard]] constexpr basic_c_str_view(null_term_t, C const* const c_str, std::size_t const len) noexcept:
        std::basic_string_view<C, T>(c_str, len)
    {
        if (std::is_constant_evaluated()) {
            verify_c_str_ptr(c_str);
            verify_c_str_data();
            verify_c_str_terminator();
        }
    }

    template <typename Cc, std::size_t N>
    requires (N > 0 && std::same_as<std::remove_const_t<Cc>, C>)
    [[nodiscard]] constexpr basic_c_str_view(Cc(&c_str)[N]):
        std::basic_string_view<C, T>(static_cast<C const*>(c_str), N - 1)
    {
        verify_c_str_data();
        verify_c_str_terminator();
    }

    // efficient but unsave function, expects c_str[< N] != 0 && c_str[N] == 0
    template <typename Cc, std::size_t N>
    requires (N > 0 && std::same_as<std::remove_const_t<Cc>, C>)
    [[nodiscard]] constexpr basic_c_str_view(null_term_t, Cc(&c_str)[N]) noexcept:
        std::basic_string_view<C, T>(static_cast<C const*>(c_str), N - 1)
    {
        if (std::is_constant_evaluated()) {
            verify_c_str_data();
            verify_c_str_terminator();
        }
    }

    template <typename A>
    [[nodiscard]] constexpr basic_c_str_view(std::basic_string<C, T, A> const& string):
        std::basic_string_view<C, T>(string.c_str(), string.size())
    {
        verify_c_str_data();
    }

    template <typename A>
    [[nodiscard]] constexpr basic_c_str_view(null_term_t, std::basic_string<C, T, A> const& string):
        std::basic_string_view<C, T>(string.c_str(), string.size())
    {
        if (std::is_constant_evaluated()) {
            verify_c_str_data();
        }
    }

    [[nodiscard]] constexpr basic_c_str_view(basic_c_str_view const&) = default;

    constexpr basic_c_str_view& operator=(std::basic_string_view<C, T> const&) noexcept = delete;
    constexpr basic_c_str_view& operator=(basic_c_str_view const&) noexcept = default;

    [[nodiscard]] constexpr C const* c_str() const noexcept {
        return std::basic_string_view<C, T>::data();
    }

    [[nodiscard]] constexpr std::size_t max_size() const noexcept {
        return std::basic_string_view<C, T>::max_size() - 1;
    }

    constexpr void remove_suffix(std::size_t) = delete;

    constexpr void swap(std::basic_string_view<C, T>&) = delete;
    constexpr void swap(basic_c_str_view& view) noexcept {
        std::basic_string_view<C, T>::swap(view);
    }

    constexpr void verify_c_str_data() const {
        cadapt::verify_c_str_data(c_str(), std::basic_string_view<C, T>::size());
    }

    constexpr void verify_c_str_terminator() const {
        cadapt::verify_c_str_terminator(c_str(), std::basic_string_view<C, T>::size());
    }
};
```

#### Deduction guides

```cpp
template <typename Cc>
basic_c_str_view(Cc*) -> basic_c_str_view<std::remove_const_t<Cc>>;

template <typename Cc, std::size_t N>
basic_c_str_view(null_term_t, Cc(&)[N]) -> basic_c_str_view<std::remove_const_t<Cc>>;

template <typename C, typename T, typename A>
basic_c_str_view(std::basic_string<C, T, A> const&) -> basic_c_str_view<C, T>;
```

### `out_c_str`

With `out_c_str` you can easily use an `std::string` as target buffer of an C-String output parameter.

`resize_for_out_ptr`
