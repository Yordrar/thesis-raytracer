#include "Dielectric.h"

Dielectric::Dielectric(Vector3 _albedo, float refraction_index)
	: Material(_albedo)
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

float Dielectric::get_discriminant(Ray ray, Vector3 normal, float ni_over_nt) {
	float dot_r_n = ray.get_direction().dot(normal);
	float discriminant = 1.0f - ni_over_nt*ni_over_nt * (1.0f-dot_r_n*dot_r_n);
	return discriminant;
}

float Dielectric::schlick(float cosine)
{
	float r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
	r0 *= r0;
	return r0 + (1.0f - r0) * powf(1.0f - cosine, 5);
}

Ray Dielectric::scatter(Ray ray, float t, Vector3 normal)
{
	float ni_over_nt = 0;
	float schlick_cosine;
	// If the cosine of the ray and the normal is < 0
	// we are scattering from outside the object
	if(ray.get_direction().dot(normal) <= 0) {
		ni_over_nt = 1.0f / refraction_index;
		schlick_cosine = -ray.get_direction().dot(normal);
	}
	else {
		// In this case we are scattering from inside the object
		// the normal needs to be flipped for snell's law to hold
		ni_over_nt = refraction_index;
		schlick_cosine = refraction_index * ray.get_direction().dot(normal);
		normal = -normal;
	}

	// The discriminant tells us if there is total internal reflection
	// which is when there is no real solution to snell's law
	// (it's because there is a sqrt in the calculation of the new ray)
	float discriminant = get_discriminant(ray, normal, ni_over_nt);
	float reflect_prob;
	if(discriminant > 0) {
		reflect_prob = schlick(schlick_cosine);
	}
	else {
		// Total internal reflection
		reflect_prob = 1.0f;
	}

	// If we reflect, we do it like a metal material with no roughness
	// If not, we calculate the scattered ray and return it
	if(Math::Randf() < reflect_prob) {
		Vector3 new_direction = ray.get_direction() - 2*ray.get_direction().dot(normal)*normal;
		return Ray(ray.get_point(t), new_direction);
	}
	else {
		Vector3 new_direction = ni_over_nt*(ray.get_direction() - ray.get_direction().dot(normal)*normal) - normal * sqrtf(discriminant);
		return Ray(ray.get_point(t), new_direction);
	}
}
