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

TEST(Math, Randf) {
	for(int i = 0; i < 100; i++) {
		float rnd = Math::Randf();
		EXPECT_GE(rnd, 0.0f);
		EXPECT_LT(rnd, 1.0f);
	}
}

TEST(Math, Deg2Rad) {
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(0), 0));
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(90), Math::PI/2.0f));
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(180), Math::PI));
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(270), 3*Math::PI/2.0f));
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(360), 2*Math::PI));
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(45), Math::PI/4.0f));
	EXPECT_TRUE(Math::Float_Eq(Math::Deg2Rad(135), 3*Math::PI/4.0f));
}

TEST(Math, Rad2Deg) {
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(0), 0));
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(Math::PI/2.0f),90));
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(Math::PI), 180));
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(3*Math::PI/2.0f), 270));
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(2*Math::PI), 360));
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(Math::PI/4.0f), 45));
	EXPECT_TRUE(Math::Float_Eq(Math::Rad2Deg(3*Math::PI/4.0f), 135));
}

TEST(Math, FastMin) {
	EXPECT_EQ(Math::Fast_Min(0.0f, 0.0f), 0.0f);
	EXPECT_EQ(Math::Fast_Min(0.0f, 1.0f), 0.0f);
	EXPECT_EQ(Math::Fast_Min(1.0f, 0.0f), 0.0f);
	EXPECT_EQ(Math::Fast_Min(0.0f, FLT_MAX), 0.0f);
	EXPECT_EQ(Math::Fast_Min(0.0f, FLT_MIN), 0.0f);
	EXPECT_EQ(Math::Fast_Min(FLT_MAX, FLT_MIN), FLT_MIN);
}

TEST(Math, FastMax) {
	EXPECT_EQ(Math::Fast_Max(0.0f, 0.0f), 0.0f);
	EXPECT_EQ(Math::Fast_Max(0.0f, 1.0f), 1.0f);
	EXPECT_EQ(Math::Fast_Max(1.0f, 0.0f), 1.0f);
	EXPECT_EQ(Math::Fast_Max(0.0f, FLT_MAX), FLT_MAX);
	EXPECT_EQ(Math::Fast_Max(0.0f, FLT_MIN), FLT_MIN);
	EXPECT_EQ(Math::Fast_Max(FLT_MAX, FLT_MIN), FLT_MAX);
}
