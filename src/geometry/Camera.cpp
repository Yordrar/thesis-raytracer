#include "Camera.h"

#include <geometry/Scatterer.h>
#include <iostream>

Camera::Camera(int width, int height, float vertical_fov)
	: Entity(Vector3(), Quaternion(0, Vector3(0, 0, -1))),
	  width(width),
	  height(height),
	  vfov(vertical_fov)
{
	recalculate_parameters();
}

void Camera::set_width(int new_width)
{
	if(width != new_width) {
		width = new_width;
		recalculate_parameters();
	}
}

void Camera::set_height(int new_height)
{
	if(height != new_height) {
		height = new_height;
		recalculate_parameters();
	}
}

void Camera::set_width_and_height(int new_width, int new_height)
{
	if(width != new_width || height != new_height) {
		width = new_width;
		height = new_height;
		recalculate_parameters();
	}
}

Vector3 Camera::get_color(float x, float y, const BVH& intersectables) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r = get_ray(u, v);
	return shoot_ray(r, intersectables, 0);
}

#define DEFAULT_EDIT_MODE_BGND_COLOR 0.1f
Vector3 Camera::get_color_preview_mode(float x, float y, const BVH& intersectables) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r = get_ray(u, v);
	Hit intersection = intersectables.get_intersection(r);
	if(intersection.is_hit() && intersection.get_normal().dot(r.get_direction()) <= 0.0f) {
		return Vector3(Math::Map(r.get_direction().dot(intersection.get_normal()), -1.0f, 0.0f, 0.5f, 0.0f));
	}
	return Vector3(DEFAULT_EDIT_MODE_BGND_COLOR);
}

void Camera::translate(float delta_x, float delta_y, float delta_z)
{
	Vector3 delta;
	delta += right*delta_x;
	delta += up*delta_y;
	delta += -orientation.get_imaginary()*delta_z;
	Entity::translate(delta);
}

void Camera::translate(Vector3 delta)
{
	Vector3 new_delta;
	new_delta += right*delta.get_x();
	new_delta += up*delta.get_y();
	new_delta += -orientation.get_imaginary()*delta.get_z();
	Entity::translate(new_delta);
}

void Camera::rotate(float euler_x, float euler_y, float euler_z)
{
	Entity::rotate(euler_x, euler_y, euler_z);
	Quaternion qx = Quaternion::create_rotation(euler_x, Vector3(1, 0, 0));
	Quaternion qy = Quaternion::create_rotation(euler_y, Vector3(0, 1, 0));
	Quaternion qz = Quaternion::create_rotation(euler_z, Vector3(0, 0, 1));
	Quaternion q = qz*(qy*qx).unit();
	up = q.apply(up);
	right = q.apply(right);

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
}

void Camera::rotate(Quaternion rotation)
{
	Entity::rotate(rotation);
	up = rotation.apply(up);
	right = rotation.apply(right);

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
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

void Camera::recalculate_parameters()
{
	half_height = tanf(Math::Deg2Rad(vfov)/2);
	half_width = static_cast<float>(width)/static_cast<float>(height) * half_height;

	plane_width = half_width * 2.0f;
	plane_height = half_height * 2.0f;

	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
}
