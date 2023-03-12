# CAdapt

CAdapt is a C++ header-only library for easy and safe use of C functions. Minimum is C++11, using C++23 brings performance improvements.

**The library is still in an early stage!**

## Usage

TODO: CMake.

## Documentation

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
