#include <gtest/gtest.h>

#include <math/Ray.h>
#include <math/Vector3.h>

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
