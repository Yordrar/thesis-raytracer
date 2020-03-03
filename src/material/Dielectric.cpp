#include "Dielectric.h"

Dielectric::Dielectric(Vector3 albedo, float refraction_index)
	: Material(albedo)
{
	this->refraction_index = refraction_index;
}

Dielectric::Dielectric(float r, float g, float b, float refraction_index)
	: Material(r, g, b)
{
	this->refraction_index = refraction_index;
}

Dielectric::~Dielectric()
{

}

Ray Dielectric::scatter(Ray ray, float t, Vector3 normal)
{
	return ray;
}
