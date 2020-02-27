#include <gtest/gtest.h>

#include <math/Ray.h>
#include <math/Vector3.h>

TEST(Ray, Constructors) {
	Vector3 origin(1, 2, 3);
	Vector3 direction(1, 1, 1);

	Ray r1(direction);
	EXPECT_EQ(r1.get_origin(), Vector3());
	EXPECT_EQ(r1.get_direction(), direction.unit());

	Ray r2(origin, direction);
	EXPECT_EQ(r2.get_origin(), origin);
	EXPECT_EQ(r2.get_direction(), direction.unit());
}

TEST(Ray, GetPointInRay) {
	Ray r(Vector3(), Vector3(1, 1, 1));

	Vector3 p = r.get_point(1.0f);
	Vector3 p_ = r.get_origin() + r.get_direction()*1.0f;
	EXPECT_EQ(p.get_x(), p_.get_x());
	EXPECT_EQ(p.get_y(), p_.get_y());
	EXPECT_EQ(p.get_z(), p_.get_z());

	p = r.get_point(4.0f);
	p_ = r.get_origin() + r.get_direction()*4.0f;
	EXPECT_EQ(p.get_x(), p_.get_x());
	EXPECT_EQ(p.get_y(), p_.get_y());
	EXPECT_EQ(p.get_z(), p_.get_z());

	p = r.get_point(-1.0f);
	p_ = r.get_origin() + r.get_direction()*-1.0f;
	EXPECT_EQ(p.get_x(), p_.get_x());
	EXPECT_EQ(p.get_y(), p_.get_y());
	EXPECT_EQ(p.get_z(), p_.get_z());

	p = r.get_point(-4.0f);
	p_ = r.get_origin() + r.get_direction()*-4.0f;
	EXPECT_EQ(p.get_x(), p_.get_x());
	EXPECT_EQ(p.get_y(), p_.get_y());
	EXPECT_EQ(p.get_z(), p_.get_z());
}
