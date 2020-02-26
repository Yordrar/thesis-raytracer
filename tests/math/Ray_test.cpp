#include <gtest/gtest.h>

#include <math/Ray.h>
#include <math/Vector3.h>

TEST(Ray, Constructors) {
	Vector3 origin(1, 2, 3);
	Vector3 direction(1, 1, 1);

	Ray r1(direction);
	EXPECT_EQ(r1.get_origin(), Vector3(0, 0, 0));
	EXPECT_EQ(r1.get_direction(), direction);

	Ray r2(origin, direction);
	EXPECT_EQ(r2.get_origin(), origin);
	EXPECT_EQ(r2.get_direction(), direction);
}

