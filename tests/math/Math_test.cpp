#include <gtest/gtest.h>
#include <math/Math.h>

#include <cfloat>

TEST(Math, FloatEq) {
	EXPECT_TRUE(Math::Float_Eq(1.1f, 1.1f));
	EXPECT_TRUE(Math::Float_Eq(1.1f, 1.1f+FLT_EPSILON));
	EXPECT_TRUE(Math::Float_Eq(1.1f, 1.1f-FLT_EPSILON));

	EXPECT_FALSE(Math::Float_Eq(1.1f, 1.2f));
	EXPECT_FALSE(Math::Float_Eq(1.1f, 1.1f+FLT_EPSILON+FLT_EPSILON));
	EXPECT_FALSE(Math::Float_Eq(1.1f, 1.1f-FLT_EPSILON-FLT_EPSILON));
	EXPECT_FALSE(Math::Float_Eq(1.0, 1.000001f));
}

TEST(Math, Clamp) {
    EXPECT_EQ(Math::Clamp(5, 1, 10), 5);
    EXPECT_EQ(Math::Clamp(0, 1, 10), 1);
    EXPECT_EQ(Math::Clamp(11, 1, 10), 10);
}

TEST(Math, Clampf) {
	EXPECT_TRUE(Math::Float_Eq(Math::Clampf(5.1f, 1.3f, 10.7f), 5.1f));
	EXPECT_TRUE(Math::Float_Eq(Math::Clampf(1.2f, 1.3f, 10.7f), 1.3f));
	EXPECT_TRUE(Math::Float_Eq(Math::Clampf(10.8f, 1.3f, 10.7f), 10.7f));
}

TEST(Math, Map) {
	EXPECT_TRUE(Math::Float_Eq(Math::Map(0.5f, 0, 1, 0, 10), 5));
	EXPECT_TRUE(Math::Float_Eq(Math::Map(0.3f, 0, 1, 0, 20), 6));
    EXPECT_TRUE(Math::Float_Eq(Math::Map(0, 0, 1, 10, 100), 10));
    EXPECT_TRUE(Math::Float_Eq(Math::Map(1, 0, 1, 10, 100), 100));
}
