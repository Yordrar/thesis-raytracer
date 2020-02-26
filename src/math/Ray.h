#pragma once

#include "Vector3.h"

class Ray
{
public:
	Ray(Vector3 direction);
	Ray(Vector3 origin, Vector3 direction);

	inline Vector3 get_origin() const {return origin;}
	inline Vector3 get_direction() const {return direction;}

private:
	Vector3 origin, direction;
};

