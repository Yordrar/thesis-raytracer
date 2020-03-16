#include <gtest/gtest.h>

#include <math/Quaternion.h>

TEST(Quaternion, Constructors) {

}

TEST(Quaternion, ApplyToVector) {
	Vector3 v(1, 0, 0);
	Quaternion q(0, 1, 0, Math::Deg2Rad(90));
	Vector3 v2 = q.apply(v);
	EXPECT_EQ(q.apply(v), Vector3(0, 0, -1));
}
