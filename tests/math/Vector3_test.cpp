#include <gtest/gtest.h>
#include <math/Vector3.h>

#include <cmath>

TEST(Vector3, GetMagnitude) {
    Vector3 v(1, 2, 3);
    ASSERT_EQ(v.get_magnitude(), static_cast<float>(sqrt(1*1 + 2*2 + 3*3)));
}

TEST(Vector3, GetSquaredMagnitude) {
	Vector3 v(1, 2, 3);
	ASSERT_EQ(v.get_squared_magnitude(), static_cast<float>(1*1 + 2*2 + 3*3));
}

TEST(Vector3, DotProduct) {
    Vector3 v1(1, 2, 3), v2(4, 5, 6);
    ASSERT_EQ(v1.dot(v2), 32);
}

TEST(Vector3, CrossProduct) {
    Vector3 v1(1, 2, 3), v2(4, 5, 6);
    Vector3 v3 = v1.cross(v2);
    EXPECT_EQ(v3.get_x(), -3); // left.y * right.z - left.z * right.y
    EXPECT_EQ(v3.get_y(), 6); // left.z * right.x - left.x * right.z
    EXPECT_EQ(v3.get_z(), -3); // left.x * right.y - left.y * right.x

    // Test the right hand rule (v3 must be orthogonal to v1 and v2)
    EXPECT_EQ(v3.dot(v1), 0);
    EXPECT_EQ(v3.dot(v2), 0);
}

TEST(Vector3, SumTwoVectors) {
    Vector3 v1(1, 2, 3), v2(4, 5, 6);
    Vector3 v3 = v1 + v2;
    EXPECT_EQ(v3.get_x(), 5);
    EXPECT_EQ(v3.get_y(), 7);
    EXPECT_EQ(v3.get_z(), 9);
    
    Vector3 v4(1, 1, 1);
    v4 += v1 + v2;
    EXPECT_EQ(v4.get_x(), 6);
    EXPECT_EQ(v4.get_y(), 8);
    EXPECT_EQ(v4.get_z(), 10);
}

TEST(Vector3, SubTwoVectors) {
	Vector3 v1(1, 2, 3), v2(4, 5, 6);
	Vector3 v3 = v1 - v2;
	EXPECT_EQ(v3.get_x(), -3);
	EXPECT_EQ(v3.get_y(), -3);
	EXPECT_EQ(v3.get_z(), -3);

	Vector3 v4(1, 1, 1);
	v4 -= v1 - v2;
	EXPECT_EQ(v4.get_x(), 4);
	EXPECT_EQ(v4.get_y(), 4);
	EXPECT_EQ(v4.get_z(), 4);
}

TEST(Vector3, MultWithScalar) {
    Vector3 v1(1, 2, 3);
    Vector3 v2 = v1 * 5;
    EXPECT_EQ(v2.get_x(), 5);
    EXPECT_EQ(v2.get_y(), 10);
    EXPECT_EQ(v2.get_z(), 15);

    v1 *= 5;
    EXPECT_EQ(v2.get_x(), 5);
    EXPECT_EQ(v2.get_y(), 10);
    EXPECT_EQ(v2.get_z(), 15);

	Vector3 v3 = 5 * v1;
	EXPECT_EQ(v3.get_x(), 25);
	EXPECT_EQ(v3.get_y(), 50);
	EXPECT_EQ(v3.get_z(), 75);
}

TEST(Vector3, DivWithScalar) {
    Vector3 v1(1, 2, 3);
    Vector3 v2 = v1 / 5.0;
    EXPECT_EQ(v2.get_x(), static_cast<float>(1/5.0));
    EXPECT_EQ(v2.get_y(), static_cast<float>(2/5.0));
    EXPECT_EQ(v2.get_z(), static_cast<float>(3/5.0));

    v1 /= 5.0;
    EXPECT_EQ(v2.get_x(), static_cast<float>(1/5.0));
    EXPECT_EQ(v2.get_y(), static_cast<float>(2/5.0));
    EXPECT_EQ(v2.get_z(), static_cast<float>(3/5.0));


	Vector3 v3 = 5.0 / v1;
	EXPECT_EQ(v3.get_x(), static_cast<float>(5.0f/v1.get_x()));
	EXPECT_EQ(v3.get_y(), static_cast<float>(5.0f/v1.get_y()));
	EXPECT_EQ(v3.get_z(), static_cast<float>(5.0f/v1.get_z()));
}

TEST(Vector3, ConvertToUnitVector) {
    Vector3 v1(1, 2, 3);
    Vector3 u = v1.unit();
	EXPECT_EQ(roundf(u.get_magnitude()), 1);

	Vector3 v2(0, 0, 0);
	u = v2.unit();
	EXPECT_EQ(roundf(u.get_magnitude()), 0);
}

TEST(Vector3, AngleBetweenOtherVector) {
    Vector3 v1(1, 2, 3), v2(4, 5, 6);
    float angle = v1.angle(v2);
    ASSERT_EQ(angle, acosf(v1.dot(v2) / (v1.get_magnitude() * v2.get_magnitude())));
}

TEST(Vector3, Equality) {
    Vector3 v1(1, 2, 3), v2(4, 5, 6), v3(1, 2, 3);
	EXPECT_TRUE(v1 == v3);
	EXPECT_FALSE(v1 == v2);
	EXPECT_FALSE(v3 == v2);
}

TEST(Vector3, Inequality) {
	Vector3 v1(1, 2, 3), v2(4, 5, 6), v3(1, 2, 3);
	EXPECT_FALSE(v1 != v3);
	EXPECT_TRUE(v1 != v2);
	EXPECT_TRUE(v3 != v2);
}

TEST(Vector3, Indexing) {
	Vector3 v1(1, 2, 3);
	EXPECT_EQ(v1[0], 1);
	EXPECT_EQ(v1[1], 2);
	EXPECT_EQ(v1[2], 3);
	EXPECT_EQ(v1[3], 0);
	EXPECT_EQ(v1[-1], 0);
}
