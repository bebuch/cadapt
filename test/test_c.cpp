#include <cadapt/c_str.hpp>

#include <gtest/gtest.h>

#include <string_view>

class in_c_str_test: public ::testing::Test {
protected:
    std::string_view ref_ = "";

    void set_ref(std::same_as<std::string_view> auto const ref) {
        ref_ = ref;
    }

    bool char_const_ptr(char const* c_str) const {
        return ref_ == c_str;
    }

    bool char_ptr(char* c_str) const {
        return ref_ == c_str;
    }
};

using cadapt::c_str;
using namespace std::literals;
using namespace cadapt::literals;

TEST_F(in_c_str_test, c_str) {
    set_ref("test"sv);
    EXPECT_TRUE(char_const_ptr(c_str("test")));

    set_ref("te\0st"sv);
    EXPECT_FALSE(char_const_ptr(c_str("te\0st")));
}

TEST_F(in_c_str_test, std_string) {
    set_ref("test"sv);
    EXPECT_TRUE(char_const_ptr(c_str("test"s)));

    set_ref("te\0st"sv);
    EXPECT_THROW(char_const_ptr(c_str("te\0st"s)), std::logic_error);
}

TEST_F(in_c_str_test, std_string_view) {
    set_ref("test"sv);
    EXPECT_TRUE(char_const_ptr(c_str("test"sv)));

    set_ref("te\0st"sv);
    EXPECT_THROW(char_const_ptr(c_str("te\0st"sv)), std::logic_error);
}

TEST_F(in_c_str_test, c_str_view) {
    set_ref("test"sv);
    EXPECT_TRUE(char_const_ptr(c_str("test"_sv)));

    set_ref("te\0st"sv);
    EXPECT_THROW(char_const_ptr(c_str("te\0st"_sv)), std::logic_error);
}
