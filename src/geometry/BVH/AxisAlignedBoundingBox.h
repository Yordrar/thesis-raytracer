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

	inline float get_surface_area() const {return surface_area;}

	inline Vector3 get_centroid() const {return (min_corner + max_corner) / 2.0f;}

	bool hit(const Ray& ray, float tmin, float tmax) const;

    static AxisAlignedBoundingBox surrounding_box(AxisAlignedBoundingBox box0, AxisAlignedBoundingBox box1);
    static int box_x_compare(const void* a, const void* b);
    static int box_y_compare(const void* a, const void* b);
    static int box_z_compare(const void* a, const void* b);

private:
	Vector3 min_corner, max_corner;
	float calculate_surface_area() const;
	float surface_area;
};

