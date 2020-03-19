#pragma once

#include "Math.h"

#include <cmath>
#include <cfloat>

class Vector3
{
public:
    Vector3();
	Vector3(float);
	Vector3(float, float, float);

	inline float get_x() const {return x;}
	inline float get_y() const {return y;}
	inline float get_z() const {return z;}
	inline void set_x(const float val) {x = val;}
	inline void set_y(const float val) {y = val;}
	inline void set_z(const float val) {z = val;}
	inline float get_magnitude() const {return sqrtf(x*x + y*y + z*z);}
	inline float get_squared_magnitude() const {return x*x + y*y + z*z;}

	inline Vector3 min(const Vector3& other) const {return Vector3(Math::Fast_Min(x, other.x), Math::Fast_Min(y, other.y), Math::Fast_Min(z, other.z));}
	inline Vector3 max(const Vector3& other) const {return Vector3(Math::Fast_Max(x, other.x), Math::Fast_Max(y, other.y), Math::Fast_Max(z, other.z));}

	Vector3 unit() const;
	inline float dot(const Vector3& other) const {return x*other.x + y*other.y + z*other.z;}
	inline Vector3 cross(const Vector3& other) const {return Vector3(y*other.z - z*other.y,
														z*other.x - x*other.z,
														x*other.y - y*other.x);}
	inline float angle(const Vector3& other) const {return acosf(this->unit().dot(other.unit()));}

	inline Vector3 operator+(const Vector3& other) const {return Vector3(x+other.x, y+other.y, z+other.z);}
	inline Vector3 operator-(const Vector3& other) const {return Vector3(x-other.x, y-other.y, z-other.z);}
	inline Vector3 operator-() const {return Vector3(-x, -y, -z);}
	inline Vector3 operator*(const float val) const {return Vector3(x*val, y*val, z*val);}
	inline Vector3 operator*(const Vector3& other) const {return Vector3(x*other.x, y*other.y, z*other.z);}
	inline Vector3 operator/(const float val) const {return *this * (1.0f/val);}
	inline Vector3 operator+=(const Vector3& other) {return Vector3(x+=other.x, y+=other.y, z+=other.z);}
	inline Vector3 operator-=(const Vector3& other) {return Vector3(x-=other.x, y-=other.y, z-=other.z);}
	inline Vector3 operator*=(const float val) {return Vector3(x*=val, y*=val, z*=val);}
	inline Vector3 operator*=(const Vector3& other) {return Vector3(x*=other.x, y*=other.y, z*=other.z);}
	inline Vector3 operator/=(const float val) {return *this *= (1.0f/val);}
	inline bool operator==(const Vector3& other) const {return Math::Float_Eq(x, other.x) && Math::Float_Eq(y, other.y) && Math::Float_Eq(z, other.z);}
	inline bool operator!=(const Vector3& other) const {return !(*this == other);}

	inline float operator[](const int index) const {
		switch(index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return 0;
		}
	}

	static Vector3 random_in_unit_sphere();

private:
    float x, y, z;
};

inline Vector3 operator*(const float& val, const Vector3& vec) {
	return vec*val;
}

inline Vector3 operator/(const float& val, const Vector3& vec) {
	return Vector3(val/vec.get_x(), val/vec.get_y(), val/vec.get_z());
}
