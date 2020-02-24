#pragma once

#include "Math.h"

#include <cmath>
#include <cfloat>

class Vector3
{
public:
    Vector3();
	Vector3(float, float, float);

	inline float get_x() {return x;}
	inline float get_y() {return y;}
	inline float get_z() {return z;}
	inline float get_magnitude() {return sqrtf(x*x + y*y + z*z);}
	inline Vector3 unit() {return *this / this->get_magnitude();}
	inline float dot(Vector3 other) {return x*other.x + y*other.y + z*other.z;}
	inline Vector3 cross(Vector3 other) {return Vector3(y*other.z - z*other.y,
														z*other.x - x*other.z,
														x*other.y - y*other.x);}
	inline float angle(Vector3 other) {return acosf(this->unit().dot(other.unit()));}
	inline Vector3 operator+(Vector3 other) {return Vector3(x+other.x, y+other.y, z+other.z);}
	inline Vector3 operator*(float val) {return Vector3(x*val, y*val, z*val);}
	inline Vector3 operator/(float val) {return *this * (1/val);}
	inline Vector3 operator+=(Vector3 other) {return Vector3(x+=other.x, y+=other.y, z+=other.z);}
	inline Vector3 operator*=(float val) {return Vector3(x*=val, y*=val, z*=val);}
	inline Vector3 operator/=(float val) {return *this * (1/val);}
	inline bool operator==(Vector3 other) {return Math::Float_Eq(x, other.x) && Math::Float_Eq(y, other.y) && Math::Float_Eq(z, other.z);}

private:
    float x, y, z;
};
