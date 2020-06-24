#include <gtest/gtest.h>

#include <geometry/Camera.h>

TEST(Camera, Constructors) {
	Camera c(10, 10);
	//EXPECT_EQ(c.get_upper_left_corner(), Vector3(-0.414214f, 0.414214f, -1.0f));
}

TEST(Camera, Rotate) {
	Camera c(10, 10);
	c.rotate(0, -45, 0);
	EXPECT_EQ(roundf(c.get_rotation_euler().get_x()), 0);
	EXPECT_EQ(roundf(c.get_rotation_euler().get_y()), -45);
	EXPECT_EQ(roundf(c.get_rotation_euler().get_z()), 0);
}

TEST(Camera, GetRay) {
	Camera c(10, 10);
	c.set_width_and_height(100, 100);
	Ray ray = c.get_ray(0.5f, 0.5f);
	EXPECT_EQ(roundf(ray.get_direction().get_x()), 0);
	EXPECT_EQ(roundf(ray.get_direction().get_y()), 0);
	EXPECT_EQ(roundf(ray.get_direction().get_z()), -1);
}
