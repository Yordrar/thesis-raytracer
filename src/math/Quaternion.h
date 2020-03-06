#pragma once

#include <math/Vector3.h>

class Quaternion
{
public:
	Quaternion(float x = 1, float y = 0, float z = 0, float w = 1);
	Quaternion(Vector3 imaginary = Vector3(1, 0, 0), float real = 1);

	inline float get_x() const {return x;}
	inline float get_y() const {return y;}
	inline float get_z() const {return z;}
	inline float get_w() const {return w;}
	inline Vector3 get_imaginary() const {
		return Vector3(x, y, z);
	}
	inline float get_magnitude() const {
		return sqrtf(w*w + get_imaginary().get_squared_magnitude());
	}
	inline float get_squared_magnitude() const {
		return w*w + get_imaginary().get_squared_magnitude();
	}

	inline Quaternion get_conjugate() const {
		return Quaternion(-x, -y, -z, w);
	}
	Quaternion get_inverse() const;

	inline Quaternion unit() const {return *this / get_magnitude();}

	inline Quaternion operator+(Quaternion other) const {
		return Quaternion(x+other.x, y+other.y, z+other.z, w+other.w);
	}
	inline Quaternion operator-(Quaternion other) const {
		return Quaternion(x-other.x, y-other.y, z-other.z, w-other.w);
	}
	inline Quaternion operator-() const {
		return Quaternion(-x, -y, -z, -w);
	}
	Quaternion operator*(Quaternion other) const;
	inline Quaternion operator/(Quaternion other) const {
		return *this * other.get_inverse();
	}

private:
	float x, y, z, w;
};

