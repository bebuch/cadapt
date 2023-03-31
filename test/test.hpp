#pragma once

#include <gtest/gtest.h>

#ifdef DISABLE_CT_TESTS
#define CT_EXPECT_TRUE(val) \
    EXPECT_TRUE(val)

#define CT_EXPECT_FALSE(val) \
    EXPECT_FALSE(val)

#define CT_EXPECT_NO_THROW(val) \
    EXPECT_NO_THROW(val)
#else
#define CT_EXPECT_TRUE(val) \
    static_assert(val); \
    EXPECT_TRUE(val)

#define CT_EXPECT_FALSE(val) \
    static_assert(!(val)); \
    EXPECT_FALSE(val)

#define CT_EXPECT_NO_THROW(val) \
    static_assert(((val), true)); \
    EXPECT_NO_THROW(val)
#endif
