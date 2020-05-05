#include "RefractiveLambertian.h"

RefractiveLambertian::~RefractiveLambertian()
{

}

Ray RefractiveLambertian::scatter(const Ray& ray, float t, const Vector3& normal)
{
	Vector3 new_ray_direction;
	if(ray.get_direction().dot(normal) < 0.0f) {
		new_ray_direction = -normal + Vector3::random_in_unit_sphere() * roughness;
	}
	else {
		new_ray_direction = normal + Vector3::random_in_unit_sphere() * roughness;
	}
	Vector3 intersection_point = ray.get_origin() + ray.get_direction()*t;
	return Ray(intersection_point, new_ray_direction);
}
