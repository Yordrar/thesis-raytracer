#include "Camera.h"

#include <geometry/Scatterer.h>
#include <iostream>
#include <material/Dielectric.h>
#include <material/Metal.h>

Camera::Camera(int width, int height, float vertical_fov)
	: Entity(Vector3(), Quaternion(0, Vector3(0, 0, -1))),
	  width(width),
	  height(height),
	  vfov(vertical_fov)
{
	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);
	aperture = 0;
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

Entity* Camera::get_object(float x, float y, const BVH& intersectables) const
{
	float u = x / float(width);
	float v = y / float(height);
	Ray r = get_ray(u, v);
	return dynamic_cast<Entity*>(intersectables.get_intersectable(r));
}

void Camera::translate_global(float delta_x, float delta_y, float delta_z)
{
	Vector3 delta;
	delta += right*delta_x;
	delta += up*delta_y;
	delta += -orientation.get_imaginary()*delta_z;
	Entity::translate_global(delta);
	recalculate_parameters();
}

void Camera::translate_global(const Vector3& delta)
{
	Vector3 new_delta;
	new_delta += right*delta.get_x();
	new_delta += up*delta.get_y();
	new_delta += -orientation.get_imaginary()*delta.get_z();
	Entity::translate_global(new_delta);
	recalculate_parameters();
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

	recalculate_parameters();
}

void Camera::rotate(const Quaternion& rotation)
{
	Entity::rotate(rotation);
	up = rotation.apply(up);
	right = rotation.apply(right);

	recalculate_parameters();
}

Camera* Camera::get_copy()
{
	Camera* cam = new Camera(width, height, vfov);
	cam->position = position;
	cam->orientation = orientation;
	cam->plane_width = plane_width;
	cam->plane_height = plane_height;
	cam->half_width = half_width;
	cam->half_height = half_height;
	cam->upper_left_corner = upper_left_corner;
	cam->aperture = aperture;
	cam->focus_dist = focus_dist;

	return cam;
}

#define MAX_DEPTH 10
Vector3 Camera::get_color_recursive(const Ray& r, const BVH& intersectables, const std::vector<Emitter*>& emitters, int depth) const
{
	Vector3 color;
	Hit intersection = intersectables.get_intersection(r);
	if(intersection.is_hit()) {
		float t = intersection.get_t();
		Material* material = intersection.get_material();
		Vector3 normal;
		Vector3 material_color;
		if(!Math::Float_Eq(intersection.get_uv().get_x(), -1) && !Math::Float_Eq(intersection.get_uv().get_y(), -1)) {
			Vector3 uv = intersection.get_uv();
			if(material->get_normal_map() && intersection.get_tangent().get_squared_magnitude() != 0.0f) {
				normal = material->get_normal(uv);
				normal.set_x(Math::Map(normal.get_x(), 0, 1, -1, 1));
				normal.set_y(Math::Map(normal.get_y(), 0, 1, -1, 1));
				normal = normal.get_x() * intersection.get_tangent() + normal.get_y() * intersection.get_bitangent() + normal.get_z() * intersection.get_normal();
			}
			else {
				normal = intersection.get_normal();
			}
			std::vector<Vector3> light_vectors;
			for(Emitter* e : emitters) {
				light_vectors.push_back(e->get_shadow_ray(r.get_point(intersection.get_t())).get_direction().unit());
			}
			material_color = material->get_color(uv, normal, light_vectors, -r.get_direction().unit());
		}
		else {
			normal = intersection.get_normal();
			material_color = material->get_albedo();
		}
		Ray new_ray = material->scatter(r, t, normal);
		Vector3 emission = material->get_emission_color(r, t, normal);
		Vector3 emitters_color;
		if(material->is_affected_by_shadow_rays())
			emitters_color = get_shadow_ray_color(new_ray.get_origin(), normal, intersectables, emitters);
		if(depth < MAX_DEPTH && new_ray.get_direction().get_squared_magnitude() != 0.0f) {
			color = emission + material_color * (emitters_color + get_color_recursive(new_ray, intersectables, emitters, depth+1));
		}
		else {
			color = emission + material_color;
		}
	} else {
		/*Vector3 unit_direction = r.get_direction().unit();
		float t = 0.5f*(unit_direction.get_y() + 1.0f);
		return (1.0f-t)*Vector3(1.0f, 1.0f, 1.0f) + t*Vector3(0.5f, 0.7f, 1.0f);*/
	}
	return color;
}

#define SHADOW_RAYS 1
Vector3 Camera::get_shadow_ray_color(Vector3 origin, Vector3 normal, const BVH& intersectables, const std::vector<Emitter*>& emitters) const
{
	Vector3 color;
	for(Emitter* e : emitters) {
		for(int i = 0; i < SHADOW_RAYS; i++) {
			Ray shadow_ray = e->get_shadow_ray(origin);
			float distance_to_emitter = e->get_distance(origin);
			Hit intersection = intersectables.get_intersection(shadow_ray);
			int num_refractions = 0;
			while(intersection.is_hit() && dynamic_cast<Dielectric*>(intersection.get_material())) {
				num_refractions++;
				shadow_ray = intersection.get_material()->scatter(shadow_ray, intersection.get_t(), intersection.get_normal());
				intersection = intersectables.get_intersection(shadow_ray);
			}
			Vector3 direction_to_emitter = e->get_shadow_ray(shadow_ray.get_origin()).get_direction();
			if(!intersection.is_hit() ||
			   (intersection.is_hit() && intersection.get_t() > distance_to_emitter)) {
				if(direction_to_emitter.dot(shadow_ray.get_direction()) > 0.99f)
					color += e->get_emission_color(origin, normal);
			}
		}
	}
	return color/(SHADOW_RAYS * Math::Fast_Max(emitters.size(), 1));
}

void Camera::recalculate_parameters()
{
	float focus_dist = (position - orientation.get_imaginary()).get_magnitude();

	half_height = tanf(Math::Deg2Rad(vfov)/2);
	half_width = static_cast<float>(width)/static_cast<float>(height) * half_height;

	plane_width = half_width * 2.0f * focus_dist;
	plane_height = half_height * 2.0f * focus_dist;

	upper_left_corner = -right*half_width + up*half_height + orientation.get_imaginary();
	upper_left_corner *= focus_dist;
}
