#pragma once

#include "Vector3.h"

class Ray
{
public:
	Ray(const Vector3& direction);
	Ray(const Vector3& origin, const Vector3& direction);

	inline Vector3 get_origin() const {return origin;}
	inline Vector3 get_direction() const {return direction;}
	inline Vector3 get_direction_inverse() const {return direction_inverse;}
	inline Vector3 get_point(float t) const {return origin + direction*t;}

private:
	Vector3 origin, direction, direction_inverse;
};

