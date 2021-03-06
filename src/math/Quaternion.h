#pragma once

#include <math/Vector3.h>

class Quaternion
{
public:
    explicit Quaternion(float x = 0, float y = 0, float z = 1, float w = 0);
    explicit Quaternion(float real = 0, Vector3 imaginary = Vector3(0, 0, 1));

	inline float get_x() const {return x;}
	inline float get_y() const {return y;}
	inline float get_z() const {return z;}
	inline float get_w() const {return w;}
	inline float get_real() const {return w;}
	inline Vector3 get_imaginary() const {
		return Vector3(x, y, z);
	}
	inline float get_magnitude() const {
		return sqrtf(w*w + x*x + y*y + z*z);
	}
	inline float get_squared_magnitude() const {
		return w*w + x*x + y*y + z*z;
	}

	inline Quaternion get_conjugate() const {
		return Quaternion(-x, -y, -z, w);
	}
	Quaternion get_inverse() const;

	// Applies the rotation represented by this quaternion
	// to a point or vector (a pure quaternion)
	// The quaternion applied must be a unit quaternion
	Vector3 apply(Vector3 point) const;

	// Applies the rotation represented by this quaternion
	// to another quaternion
	// The quaternion applied must be a unit quaternion
	inline Quaternion apply(Quaternion other) const {return *this * other * get_conjugate();}

	// Interpolates this quaternion with another one
	// using spherical interpolation
	Quaternion slerp(Quaternion other, float t) const;

	inline Quaternion unit() const {return *this / get_magnitude();}

	static Quaternion create_rotation(float angle, Vector3 axis);

	Vector3 euler_angles() {
		Vector3 euler_angles;

		// pitch (x-axis rotation)
		float sinr_cosp = 2 * (w * x + y * z);
		float cosr_cosp = 1 - 2 * (x * x + y * y);
		euler_angles.set_x(std::atan2(sinr_cosp, cosr_cosp));

		// yaw (y-axis rotation)
		float siny = 2 * (w * y - z * x);
		if (std::abs(siny) >= 1)
			euler_angles.set_y(static_cast<float>(std::copysign(M_PI / 2, siny))); // use 90 degrees if out of range
		else
			euler_angles.set_y(std::asin(siny));

		// roll (z-axis rotation)
		float siny_cosp = 2 * (w * z + x * y);
		float cosy_cosp = 1 - 2 * (y * y + z * z);
		euler_angles.set_z(std::atan2(siny_cosp, cosy_cosp));

		return euler_angles;
	}

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
		float invVal = 1.0f/val;
		return Quaternion(x*invVal, y*invVal, z*invVal, w*invVal);
	}
	inline bool operator==(const Quaternion& q) const {
		return x == q.x && y == q.y && z == q.z && w == q.w;
	}
	inline bool operator!=(const Quaternion& q) const {
		return !(*this == q);
	}

private:
	float x, y, z, w;
};

