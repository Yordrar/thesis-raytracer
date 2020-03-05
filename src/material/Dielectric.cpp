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

bool Dielectric::get_discriminant(Ray ray, Vector3 normal, float ni_over_nt) {
	float dot_r_n = ray.get_direction().dot(normal);
	float discriminant = 1.0f - ni_over_nt*ni_over_nt * (1.0f-dot_r_n*dot_r_n);
	return discriminant;
}
Ray Dielectric::scatter(Ray ray, float t, Vector3 normal)
{
	float ni_over_nt = 0;
	// If the cosine of the ray and the normal is < 0
	// we are scattering from outside the object
	if(ray.get_direction().dot(normal) <= 0) {
		ni_over_nt = 1.0f / refraction_index;
	}
	else {
		// In this case we are scattering from inside the object
		// the normal needs to be flipped for snell's law to hold
		ni_over_nt = refraction_index;
		normal = -normal;
	}

	// The discriminant tells us if there is total internal reflection
	// which is when there is no real solution to snell's law
	// (it's because there is a sqrt in the calculation of the new ray)
	float discriminant = get_discriminant(ray, normal, ni_over_nt);
	if(discriminant > 0) {
		Vector3 new_direction = ni_over_nt*(ray.get_direction() - ray.get_direction().dot(normal)*normal) - normal * sqrt(discriminant);
		return Ray(ray.get_point(t), new_direction);
	}
	else {
		// Total internal reflection
		Vector3 new_direction = ray.get_direction() - 2*ray.get_direction().dot(normal)*normal;
		return Ray(ray.get_point(t), new_direction);
	}
}
