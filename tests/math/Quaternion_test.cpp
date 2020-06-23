#include <gtest/gtest.h>

#include <math/Quaternion.h>

TEST(Quaternion, Constructors) {

}

TEST(Quaternion, Conjugate) {
	Quaternion v(1, 2, 3, 4);
	EXPECT_EQ(v.get_conjugate(), Quaternion(-1, -2, -3, 4));
}

TEST(Quaternion, Magnitude) {
	Quaternion v(1, 2, 3, 4);
	EXPECT_TRUE(Math::Float_Eq(v.get_magnitude(), sqrtf(30.0f)));
	EXPECT_TRUE(Math::Float_Eq(v.get_squared_magnitude(), 30.0f));
}

TEST(Quaternion, EulerAngles) {
	Quaternion v = Quaternion::create_rotation(90, Vector3(0, 1, 0));
	EXPECT_EQ(v, Quaternion(cosf(Math::Deg2Rad(90/2.0f)), sinf(Math::Deg2Rad(90/2.0f)) * Vector3(0, 1, 0).unit()));
}

TEST(Quaternion, Inverse) {
	Quaternion v(1, 2, 3, 4);
	float squared_magnitude = v.get_squared_magnitude();
	EXPECT_EQ(v.get_inverse(), Quaternion(4/squared_magnitude, -Vector3(1, 2, 3)/squared_magnitude));
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

TEST(Quaternion, Operators) {
	Quaternion v1(1, 2, 3, 4);
	Quaternion v2(5, 6, 7, 8);

	EXPECT_EQ(v1+v2, Quaternion(6, 8, 10, 12));
	EXPECT_EQ(v2-v1, Quaternion(4, 4, 4, 4));
	EXPECT_EQ(-v1, Quaternion(-1, -2, -3, -4));
	EXPECT_EQ(v1*v2, Quaternion(v1.get_w()*v2.get_w() - v1.get_imaginary().dot(v2.get_imaginary()),
								v1.get_w()*v2.get_imaginary() + v2.get_w()*v1.get_imaginary() + v1.get_imaginary().cross(v2.get_imaginary())));
	EXPECT_EQ(v1*4, Quaternion(4, 8, 12, 16));
	EXPECT_EQ(v1/4, Quaternion(1/4.0f, 2/4.0f, 3/4.0f, 4/4.0f));
	EXPECT_EQ(v1!=v2, true);
}
