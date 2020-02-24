#include <gtest/gtest.h>
#include <math/Math.h>

#include <cfloat>

TEST(Math, FloatEq) {
    EXPECT_TRUE(Math::Float_Eq(1.1, 1.1));
    EXPECT_TRUE(Math::Float_Eq(1.1, 1.1+FLT_EPSILON));
    EXPECT_TRUE(Math::Float_Eq(1.1, 1.1-FLT_EPSILON));

    EXPECT_FALSE(Math::Float_Eq(1.1, 1.2));
    EXPECT_FALSE(Math::Float_Eq(1.1, 1.1+FLT_EPSILON+FLT_EPSILON));
    EXPECT_FALSE(Math::Float_Eq(1.1, 1.1-FLT_EPSILON-FLT_EPSILON));
    EXPECT_FALSE(Math::Float_Eq(1.0, 1.00001));
}

TEST(Math, Clamp) {
    EXPECT_EQ(Math::Clamp(5, 1, 10), 5);
    EXPECT_EQ(Math::Clamp(0, 1, 10), 1);
    EXPECT_EQ(Math::Clamp(11, 1, 10), 10);
}

TEST(Math, Clampf) {
    EXPECT_TRUE(Math::Float_Eq(Math::Clampf(5.1, 1.3, 10.7), 5.1));
    EXPECT_TRUE(Math::Float_Eq(Math::Clampf(1.2, 1.3, 10.7), 1.3));
    EXPECT_TRUE(Math::Float_Eq(Math::Clampf(10.8, 1.3, 10.7), 10.7));
}

TEST(Math, Map) {
    EXPECT_TRUE(Math::Float_Eq(Math::Map(0.5, 0, 1, 0, 10), 5));
    EXPECT_TRUE(Math::Float_Eq(Math::Map(0.3, 0, 1, 0, 20), 6));
    EXPECT_TRUE(Math::Float_Eq(Math::Map(0, 0, 1, 10, 100), 10));
    EXPECT_TRUE(Math::Float_Eq(Math::Map(1, 0, 1, 10, 100), 100));
}