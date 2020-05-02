#include "RefractiveLambertian.h"

RefractiveLambertian::RefractiveLambertian(const Vector3& albedo, float opacity)
	: Material(albedo),
	  opacity(opacity)
{

}

RefractiveLambertian::RefractiveLambertian(float r, float g, float b, float opacity)
	: Material(r, g, b),
	  opacity(opacity)
{

}

RefractiveLambertian::~RefractiveLambertian()
{

}

Ray RefractiveLambertian::scatter(const Ray& ray, float t, const Vector3& normal)
{
	Vector3 new_ray_direction;
	if(ray.get_direction().dot(normal) < 0.0f) {
		new_ray_direction = -normal + Vector3::random_in_unit_sphere() * opacity;
	}
	else {
		new_ray_direction = normal + Vector3::random_in_unit_sphere() * opacity;
	}
	Vector3 intersection_point = ray.get_origin() + ray.get_direction()*t;
	return Ray(intersection_point, new_ray_direction);
}
