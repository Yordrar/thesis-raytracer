#pragma once

#include <math/Vector3.h>

class Quaternion
{
public:
	Quaternion(float x = 1, float y = 0, float z = 0, float w = 1);
	Quaternion(Vector3 imaginary = Vector3(1, 0, 0), float real = 1);
	Quaternion(float real = 1, Vector3 imaginary = Vector3(1, 0, 0));

	inline float get_x() const {return x;}
	inline float get_y() const {return y;}
	inline float get_z() const {return z;}
	inline float get_w() const {return w;}
	inline float get_real() const {return w;}
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

	// Applies the rotation represented by this quaternion
	// to a point or vector (a pure quaternion)
	Vector3 apply(Vector3 point) const;

	// Applies the rotation represented by this quaternion
	// to another quaternion
	inline Quaternion apply(Quaternion other) const {return *this * other * get_inverse();}

	// Interpolates this quaternion with another one
	// using spherical interpolation
	Quaternion slerp(Quaternion other, float t) const;

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
	Quaternion operator*(float val) const {
		return Quaternion(x*val, y*val, z*val, w*val);
	}
	inline Quaternion operator/(Quaternion other) const {
		return *this * other.get_inverse();
	}
	inline Quaternion operator/(float val) const {
		return Quaternion(w/val, Vector3(x, y, z) / val);
	}

private:
	float x, y, z, w;
};

