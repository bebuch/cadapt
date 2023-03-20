#include <cadapt/c_str.hpp>

void c_fn_in_string(char const*){}

void test_c_str(){
    using namespace std::literals;
    using namespace cadapt::literals;
    using cadapt::c_str;

    c_fn_in_string(c_str(""s));
    c_fn_in_string(c_str(""_sv));
    c_fn_in_string(c_str(""sv));

    char c1[1] = "";
    c_fn_in_string(c_str(c1));
    c_fn_in_string(c_str(std::move(c1)));

    char const c2[1] = "";
    c_fn_in_string(c_str(c2));
    c_fn_in_string(c_str(std::move(c2)));

    char* c3 = c1;
    c_fn_in_string(c_str(c3));
    c_fn_in_string(c_str(std::move(c3)));

    char const* c4 = c2;
    c_fn_in_string(c_str(c4));
    c_fn_in_string(c_str(std::move(c4)));
}
