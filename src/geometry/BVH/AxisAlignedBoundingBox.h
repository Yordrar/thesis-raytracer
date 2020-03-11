#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(Vector3 min, Vector3 max);

	inline Vector3 get_min_corner() const {return min_corner;}
	inline Vector3 get_max_corner() const {return max_corner;}
	inline void set_min_corner(Vector3 min) {min_corner = min;}
	inline void set_max_corner(Vector3 max) {max_corner = max;}

	bool hit(Ray ray, float tmin, float tmax) const;

	static AxisAlignedBoundingBox surrounding_box(AxisAlignedBoundingBox box0, AxisAlignedBoundingBox box1);

private:
	Vector3 min_corner, max_corner;
};

