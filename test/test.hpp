#pragma once

#include <gtest/gtest.h>

#define CT_EXPECT_TRUE(val) \
    static_assert(val); \
    EXPECT_TRUE(val)

#define CT_EXPECT_FALSE(val) \
    static_assert(!(val)); \
    EXPECT_FALSE(val)
