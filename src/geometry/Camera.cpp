#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 direction, int width, int height)
	: position(position),
	  direction(direction),
	  width(width),
	  height(height)
{

}

Vector3 Camera::get_color(float x, float y, std::list<Intersectable*> intersectables) const
{
	float vfov = 90.0f;
	float half_height = tanf(Math::Deg2Rad(vfov)/2);
	float half_width = static_cast<float>(width)/static_cast<float>(height) * half_height;
	float plane_width = half_width * 2.0f;
	float plane_height = half_height * 2.0f;

	Vector3 upper_left_corner = Vector3(-half_width, half_height, -1);
	float u = x / float(width);
	float v = y / float(height);
	Ray r(position, upper_left_corner + Vector3(u*plane_width, -v*plane_height, 0));
	return shoot_ray(r, intersectables, 0);
}

#define MAX_DEPTH 50
Vector3 Camera::shoot_ray(Ray r, const std::list<Intersectable*> intersectables, int depth) const
{
	Vector3 color;
	float min_t = FLT_MAX;
	Intersectable* intersected = nullptr;
	for(auto e : intersectables) {
		float t = e->get_intersection(r);
		if(t > 0.0f && t < min_t) {
			min_t = t;
			intersected = e;
		}
	}
	if(intersected) {
		Material* material = intersected->get_material();
		Ray new_ray = material->scatter(r, min_t, intersected->get_normal(r.get_point(min_t)));
		if(depth < MAX_DEPTH)
			color = material->get_albedo() / 255.0f * shoot_ray(new_ray, intersectables, depth+1);
		else
			color = Vector3();
	}
	else {
		Vector3 unit_direction = r.get_direction().unit();
		float t = 0.5f*(unit_direction.get_y() + 1.0f);
		color = (1.0f-t)*Vector3(1.0f, 1.0f, 1.0f) + t*Vector3(0.5f, 0.7f, 1.0f);
	}
	return color;
}
