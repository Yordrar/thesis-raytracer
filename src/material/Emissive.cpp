#include "Emissive.h"

Emissive::Emissive(const Vector3& _albedo, float intensity)
	: Material(_albedo)
{
	this->intensity = intensity;
}

Emissive::Emissive(float r, float g, float b, float intensity)
	: Material(r, g, b)
{
	this->intensity = intensity;
}

Emissive::~Emissive()
{

}

Ray Emissive::scatter(const Ray& ray, float t, const Vector3& normal)
{
	return Ray(ray.get_point(t), Vector3());
}

Vector3 Emissive::get_emission_color(const Ray& ray, float t, const Vector3& normal)
{
	return albedo * intensity;
}
