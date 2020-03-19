#include "Lambertian.h"

Lambertian::~Lambertian()
{

}

Ray Lambertian::scatter(const Ray& ray, float t, const Vector3& normal)
{
	Vector3 intersection_point = ray.get_origin() + ray.get_direction()*t;
	Vector3 new_ray_direction = normal + Vector3::random_in_unit_sphere();
	return Ray(intersection_point, new_ray_direction);
}

