# CAdapt

CAdapt is a C++ header only library for simplified and safe use of C functions. The minimum requirement is C++20.

The library has wrapper support for `QString` if Qt6 is found on the system.

**The library is still in an early stage!**

The use of C++23 improves performance at runtime for `out_c_str()` and `inout_c_str()`. Buffers are left uninitialized when enlarging. This is realized by using C++23 `resize_and_overwrite()` instead of the older `resize()`.

## Usage

TODO: CMake.

## Documentation

All tools are in the `namespace cadapt`, user-defined-literals are in the `inline namespace cadapt::literals`.

Currently, the library includes the following tools:

- `basic_c_str_view`, a `std::basic_string_view` which is guaranteed to be a completely valid null-terminated C-string
    - `""_sv` const-evaluated user-defined-literal
    - `c_str_view` (`"text"_sv`) based on `std::string_view`
    - `wc_str_view` (`L"text"_sv`) based on `std::wstring_view`
    - `u32c_str_view` (`U"text"_sv`) based on `std::u32string_view`
    - `u16c_str_view` (`u"text"_sv`) based on `std::u16string_view`
    - `u8c_str_view` (`u8"text"_sv`) based on `std::u8string_view`
- `c_str` and `unverified_c_str`
    - supports `basic_c_str_view`, `std::basic_string_view`, `std::basic_string` and `QString`

The `QString` versions use internally conversions from and to `std::basic_string` and are therefore slower and partly have a slightly changed interface. Use the normal `std::basic_string` versions by default. If data is available as `QString`, then use the wrapper versions, as they are safer and more efficient than a manual conversion.

### C sample functions

In the following C example functions are introduced. These will be called later with CAdapt.

```c
#include <string.h>

void get_name(char* const buffer, int const buffer_size) {
    char const text[] = "Jane Doe";
    strcpy(, text);
}
```

#### `void get_name(buffer, buffer_size)`


### `out_c_str`

With `out_c_str` you can easily use an `std::string` as target buffer of an C-String output parameter.

`resize_for_out_ptr`
