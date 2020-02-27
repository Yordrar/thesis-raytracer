#include <gtest/gtest.h>

#include <geometry/Sphere.h>
#include <math/Vector3.h>
#include <math/Math.h>
#include <math/Ray.h>

TEST(Sphere, Constructors) {
	Sphere s1;
	EXPECT_EQ(s1.get_position(), Vector3(0, 0, 0));
	EXPECT_EQ(s1.get_radius(), 0);

	Sphere s2(Vector3(1, 2, 3));
	EXPECT_EQ(s2.get_position(), Vector3(1, 2, 3));
	EXPECT_EQ(s2.get_radius(), 0);

	Sphere s3(Vector3(4, 5, 6), 5.5f);
	EXPECT_EQ(s3.get_position(), Vector3(4, 5, 6));
	EXPECT_EQ(s3.get_radius(), 5.5f);
}

TEST(Sphere, Intersect) {
	Ray r(Vector3(0, 0, 0), Vector3(0, 0, 1));
	Sphere s(Vector3(0, 0, 5), 2);

	EXPECT_TRUE(Math::Float_Eq(s.get_intersection(r), 3.0f));

	s.set_position(Vector3(5, 0, 0));
	EXPECT_EQ(s.get_intersection(r), -1.0f);
}
