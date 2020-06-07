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
	inline void set_min_corner(const Vector3& min) {min_corner = min;}
	inline void set_max_corner(const Vector3& max) {max_corner = max;}

    float get_surface_area() const;

	bool hit(const Ray& ray, float tmin, float tmax) const;

    static AxisAlignedBoundingBox surrounding_box(AxisAlignedBoundingBox box0, AxisAlignedBoundingBox box1);
    static int box_x_compare(const void* a, const void* b);
    static int box_y_compare(const void* a, const void* b);
    static int box_z_compare(const void* a, const void* b);

private:
	Vector3 min_corner, max_corner;
};

