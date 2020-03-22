#include "Camera.h"

#include <geometry/Scatterer.h>
#include <iostream>

Camera::Camera(int width, int height, float vertical_fov)
	: Entity(Vector3(), Quaternion(0, Vector3(0, 0, -1))),
	  width(width),
	  height(height),
	  vfov(vertical_fov)
{
	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);
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

Vector3 Camera::get_color(float x, float y, const BVH& intersectables, const std::vector<Emitter*>& emitters) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r = get_ray(u, v);
	return get_color_recursive(r, intersectables, emitters, 0).clamp(0, 1);
}

#define DEFAULT_EDIT_MODE_BGND_COLOR 0.1f
Vector3 Camera::get_color_preview(float x, float y, const BVH& intersectables) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r = get_ray(u, v);
	Hit intersection = intersectables.get_intersection(r);
	float intersection_normal_dot_ray = intersection.get_normal().dot(r.get_direction());
	if(intersection.is_hit() && intersection_normal_dot_ray <= 0.0f) {
		return Vector3(Math::Map(intersection_normal_dot_ray, -1.0f, 0.0f, 0.5f, 0.0f));
	}
	return Vector3(DEFAULT_EDIT_MODE_BGND_COLOR);
}

void Camera::translate_global(float delta_x, float delta_y, float delta_z)
{
	Vector3 delta;
	delta += right*delta_x;
	delta += up*delta_y;
	delta += -orientation.get_imaginary()*delta_z;
	Entity::translate_global(delta);
}

void Camera::translate_global(const Vector3& delta)
{
	Vector3 new_delta;
	new_delta += right*delta.get_x();
	new_delta += up*delta.get_y();
	new_delta += -orientation.get_imaginary()*delta.get_z();
	Entity::translate_global(new_delta);
}

void Camera::rotate(float euler_x, float euler_y, float euler_z)
{
	Entity::rotate_global(euler_x, euler_y, euler_z);
	Quaternion qx = Quaternion::create_rotation(euler_x, Vector3(1, 0, 0));
	Quaternion qy = Quaternion::create_rotation(euler_y, Vector3(0, 1, 0));
	Quaternion qz = Quaternion::create_rotation(euler_z, Vector3(0, 0, 1));
	Quaternion q = qz*(qy*qx).unit();
	up = q.apply(up);
	right = q.apply(right);

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
}

void Camera::rotate(const Quaternion& rotation)
{
	Entity::rotate(rotation);
	up = rotation.apply(up);
	right = rotation.apply(right);

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
}

#define MAX_DEPTH 10
Vector3 Camera::get_color_recursive(const Ray& r, const BVH& intersectables, const std::vector<Emitter*>& emitters, int depth) const
{
	Vector3 color;
	Hit intersection = intersectables.get_intersection(r);
	if(intersection.is_hit()) {
		float t = intersection.get_t();
		Material* material = intersection.get_material();
		Ray new_ray = material->scatter(r, t, intersection.get_normal());
		Vector3 emitted_color = get_shadow_ray_color(new_ray.get_origin(), intersectables, emitters);
		Vector3 texture_map_color(1);
		if(intersection.get_texture()) {
			Vector3 uv = intersection.get_uv();
			Image* texture_map = intersection.get_texture();
			texture_map_color = texture_map->get_pixel_color(static_cast<int>(uv.get_x() * texture_map->get_width()),
															 static_cast<int>((1.0f-uv.get_y()) * texture_map->get_height()));
		}
		if(depth < MAX_DEPTH)
			color = emitted_color + material->get_albedo() * texture_map_color * get_color_recursive(new_ray, intersectables, emitters, depth+1);
		else
			color = emitted_color;
	} else {
		Vector3 unit_direction = r.get_direction().unit();
		float t = 0.5f*(unit_direction.get_y() + 1.0f);
		return (1.0f-t)*Vector3(1.0f, 1.0f, 1.0f) + t*Vector3(0.5f, 0.7f, 1.0f);
	}
	return color;
}

#define SHADOW_RAYS 1
Vector3 Camera::get_shadow_ray_color(Vector3 origin, const BVH& intersectables, const std::vector<Emitter*>& emitters) const
{
	Vector3 color(0);
	for(Emitter* e : emitters) {
		for(int i = 0; i < SHADOW_RAYS; i++) {
			Vector3 shadow_ray_dir = dynamic_cast<Entity*>(e)->get_position() - origin + Vector3::random_in_unit_sphere();
			float shadow_ray_dir_magnitude = shadow_ray_dir.get_magnitude();
			Ray shadow_ray(origin, shadow_ray_dir);
			Hit intersection = intersectables.get_intersection(shadow_ray);
			if(!intersection.is_hit() ||
			   (intersection.is_hit() && intersection.get_t() > shadow_ray_dir_magnitude)) {
				color += e->get_emission_color(origin);
			}
		}
	}
	return color/(SHADOW_RAYS * Math::Fast_Max(emitters.size(), 1));
}

void Camera::recalculate_parameters()
{
	half_height = tanf(Math::Deg2Rad(vfov)/2);
	half_width = static_cast<float>(width)/static_cast<float>(height) * half_height;

	plane_width = half_width * 2.0f;
	plane_height = half_height * 2.0f;

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
}
