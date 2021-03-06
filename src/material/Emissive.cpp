#include "Emissive.h"

Emissive::~Emissive()
{

}

Ray Emissive::scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity)
{
	return Ray(ray.get_point(t), Vector3(0));
}

Vector3 Emissive::get_emission_color(const Ray& ray, float t, const Vector3& normal)
{
    return (albedo * (intensity / (ray.get_point(t) - ray.get_origin()).get_squared_magnitude())).clamp(0, 1.0f);
}
