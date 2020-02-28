#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 direction, int width, int height)
	: position(position),
	  direction(direction),
	  width(width),
	  height(height)
{

}

Vector3 Camera::get_color(float x, float y, std::list<Entity*> entities) const
{
	float fov = 90.0f;
	float fovx = 2*atanf(width/(2*fov));
	float fovy = 2*atanf(height/(2*fov));

	float plane_width = 2;
	float plane_height = 2;
	if(fovx > fovy) {
		plane_height = height*(plane_width/width);
	}
	else if(fovx < fovy) {
		plane_width = width*(plane_height/height);
	}
	Vector3 upper_left_corner = Vector3(0-plane_width/2, 0+plane_height/2, -1);
	float u = x / float(width);
	float v = y / float(height);
	Ray r(position, upper_left_corner + Vector3(u*plane_width, -v*plane_height, 0));
	return shoot_ray(r, entities);
}

Vector3 Camera::shoot_ray(Ray r, const std::list<Entity*> entities) const
{
	Vector3 color;
	float min_t = FLT_MAX;
	Entity* intersected_e = nullptr;
	for(auto e : entities) {
		float t = e->get_intersection(r);
		if(t > 0.0f && t < min_t) {
			min_t = t;
			intersected_e = e;
		}
	}
	if(intersected_e) {
		Vector3 intersection_point = r.get_point(min_t);
		Vector3 normal = intersected_e->get_normal(intersection_point);
		Vector3 new_ray_direction = intersection_point + normal + Vector3::create_random_in_unit_sphere();
		new_ray_direction -= intersection_point;
		Ray new_ray(intersection_point, new_ray_direction);
		color = 0.5f * shoot_ray(new_ray, entities);
	}
	else {
		Vector3 unit_direction = r.get_direction().unit();
		float t = 0.5f*(unit_direction.get_y() + 1.0f);
		color = (1.0f-t)*Vector3(1.0f, 1.0f, 1.0f) + t*Vector3(0.5f, 0.7f, 1.0f);
	}
	return color;
}
