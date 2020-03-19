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

bool AxisAlignedBoundingBox::hit(const Ray& ray, float tmin, float tmax) const
{
#if 1
	// Andrew Kesler's AABB intersection code
	Vector3 ray_origin = ray.get_origin();
	for(int i = 0; i < 3; i++) {
		float invD = ray.get_direction_inverse()[i];
		float t0 = (min_corner[i] - ray_origin[i]) * invD;
		float t1 = (max_corner[i] - ray_origin[i]) * invD;
		if(invD < 0.0f)
			std::swap(t0, t1);
		tmin = Math::Fast_Max(t0, tmin);
		tmax = Math::Fast_Min(t1, tmax);
		if (tmax < tmin)
			return false;
	}
	return true;
#else
	Vector3 t0 = (min_corner - ray.get_origin()) * ray.get_direction_inverse();
	Vector3 t1 = (max_corner - ray.get_origin()) * ray.get_direction_inverse();
	Vector3 vmin = t0.min(t1);
	Vector3 vmax = t0.max(t1);

	return Math::Fast_Max(vmin.get_x(), Math::Fast_Max(vmin.get_y(), vmin.get_z())) <=
			Math::Fast_Min(vmax.get_x(), Math::Fast_Min(vmax.get_y(), vmax.get_z()));
#endif
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::surrounding_box(AxisAlignedBoundingBox box0, AxisAlignedBoundingBox box1)
{
	return AxisAlignedBoundingBox(box0.get_min_corner().min(box1.get_min_corner()),
								  box0.get_max_corner().max(box1.get_max_corner()));
}
