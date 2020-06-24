#include "AxisAlignedBoundingBox.h"

#include <algorithm>

#include <geometry/Intersectable.h>

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{

}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Vector3 min, Vector3 max)
	: min_corner(min),
	  max_corner(max)
{
	surface_area = calculate_surface_area();
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

int AxisAlignedBoundingBox::box_x_compare(const void* a, const void* b) {
    Intersectable* ah = *(Intersectable**)a;
    Intersectable* bh = *(Intersectable**)b;

    AxisAlignedBoundingBox box_left = ah->get_bounding_box();
    AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if (box_left.get_centroid().get_x() < box_right.get_centroid().get_x())
        return -1;
    else
        return 1;
}

int AxisAlignedBoundingBox::box_y_compare(const void* a, const void* b) {
    Intersectable* ah = *(Intersectable**)a;
    Intersectable* bh = *(Intersectable**)b;

    AxisAlignedBoundingBox box_left = ah->get_bounding_box();
    AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if (box_left.get_centroid().get_y() < box_right.get_centroid().get_y())
        return -1;
    else
        return 1;
}

int AxisAlignedBoundingBox::box_z_compare(const void* a, const void* b) {
    Intersectable* ah = *(Intersectable**)a;
    Intersectable* bh = *(Intersectable**)b;

    AxisAlignedBoundingBox box_left = ah->get_bounding_box();
    AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if (box_left.get_centroid().get_z() < box_right.get_centroid().get_z())
        return -1;
    else
		return 1;
}

float AxisAlignedBoundingBox::calculate_surface_area() const
{
	float xy_plane_area = (max_corner.get_x() - min_corner.get_x()) * (max_corner.get_y() - min_corner.get_y());
	float xz_plane_area = (max_corner.get_x() - min_corner.get_x()) * (max_corner.get_z() - min_corner.get_z());
	float yz_plane_area = (max_corner.get_y() - min_corner.get_y()) * (max_corner.get_z() - min_corner.get_z());

	return 2*(xy_plane_area + xz_plane_area + yz_plane_area);
}
