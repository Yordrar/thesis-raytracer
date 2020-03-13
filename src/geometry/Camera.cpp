#include "Camera.h"

#include <geometry/Scatterer.h>
#include <iostream>

Camera::Camera(Vector3 position, Vector3 direction, int width, int height, float vertical_fov)
	: position(position),
	  direction(direction),
	  width(width),
	  height(height),
	  vfov(vertical_fov)
{
	float half_height = tanf(Math::Deg2Rad(vfov)/2);
	float half_width = static_cast<float>(width)/static_cast<float>(height) * half_height;

	plane_width = half_width * 2.0f;
	plane_height = half_height * 2.0f;
	upper_left_corner = Vector3(-half_width, half_height, -1);
}

Vector3 Camera::get_color(float x, float y, const BVH& intersectables) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r(position, upper_left_corner + Vector3(u*plane_width, -v*plane_height, 0));
	return shoot_ray(r, intersectables, 0);
}

#define DEFAULT_EDIT_MODE_BGND_COLOR 0.1f
Vector3 Camera::get_color_edit_mode(float x, float y, const BVH& intersectables) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r(position, upper_left_corner + Vector3(u*plane_width, -v*plane_height, 0));
	Hit intersection = intersectables.get_intersection(r);
	if(intersection.is_hit()) {
		return Vector3(Math::Map(r.get_direction().dot(intersection.get_normal()), -1.0f, 1.0f, 0.5f, 0.0f));
	}
	return Vector3(DEFAULT_EDIT_MODE_BGND_COLOR);
}

#define MAX_DEPTH 10
Vector3 Camera::shoot_ray(Ray r, const BVH& intersectables, int depth) const
{
	Vector3 color;
	Hit intersection = intersectables.get_intersection(r);
	if(intersection.is_hit()) {
		float t = intersection.get_t();
		Material* material = intersection.get_material();
		Ray new_ray = material->scatter(r, t, intersection.get_normal());
		if(depth < MAX_DEPTH)
			color = material->get_albedo() / 255.0f * shoot_ray(new_ray, intersectables, depth+1);
		else
			color = Vector3();
	}
	else {
		float t = 0.5f*(r.get_direction().get_y() + 1.0f);
		color = (1.0f-t)*Vector3(1.0f, 1.0f, 1.0f) + t*Vector3(0.5f, 0.7f, 1.0f);
	}
	return color;
}
