#include <gtest/gtest.h>

#include <math/Quaternion.h>

TEST(Quaternion, Constructors) {

}

TEST(Quaternion, ApplyToVector) {
	Quaternion v(1, 0, 0, 0);
	Quaternion q = Quaternion::create_rotation(90, Vector3(0, 1, 0));
	Quaternion v2 = q.apply(v);
	EXPECT_EQ(v2.get_imaginary(), Vector3(0, 0, -1));
	EXPECT_EQ(v2.get_real(), 0);
}

TEST(Quaternion, CombineRotations) {
	Quaternion v(1, 0, 0, 0);
	Quaternion q1 = Quaternion::create_rotation(90, Vector3(0, 1, 0));
	Quaternion q2 = Quaternion::create_rotation(90, Vector3(0, 0, 1));
	Quaternion q3 = Quaternion::create_rotation(90, Vector3(1, 0, 0));
	Quaternion q4 = Quaternion::create_rotation(90, Vector3(0, 0, 1));
	Quaternion q5 = Quaternion::create_rotation(-90, Vector3(0, 1, 0));
	Quaternion q = (q5*(q4*(q3*(q2*q1)))).unit();
	Quaternion v2 = q.apply(v);
	EXPECT_EQ(v2.get_imaginary(), Vector3(0, 0, -1));
	EXPECT_EQ(v2.get_real(), 0);
}
