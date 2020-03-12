#include "AxisAlignedBoundingBox.h"

#include <algorithm>

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{

}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Vector3 min, Vector3 max)
	: min_corner(min),
	  max_corner(max)
{

}

bool AxisAlignedBoundingBox::hit(Ray ray, float tmin, float tmax) const
{
	// Andrew Kesler's AABB intersection code
	Vector3 ray_origin = ray.get_origin();
	Vector3 ray_dir = ray.get_direction();
	for(int i = 0; i < 3; i++) {
		float invD = 1.0f / ray_dir[i];
		float t0 = (min_corner[i] - ray_origin[i]) * invD;
		float t1 = (max_corner[i] - ray_origin[i]) * invD;
		if(invD < 0.0f)
			std::swap(t0, t1);
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin)
			return false;
	}
	return true;
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::surrounding_box(AxisAlignedBoundingBox box0, AxisAlignedBoundingBox box1)
{
	return AxisAlignedBoundingBox(box0.get_min_corner().min(box1.get_min_corner()),
								  box0.get_max_corner().max(box1.get_max_corner()));
}
