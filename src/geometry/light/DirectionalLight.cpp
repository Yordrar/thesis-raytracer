#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vector3 _albedo, Vector3 direction)
	: Emitter(_albedo),
	  direction(direction)
{

}

DirectionalLight::~DirectionalLight()
{

}

Vector3 DirectionalLight::get_emission_color(Vector3 _position) const
{
    return albedo;
}

Ray DirectionalLight::get_shadow_ray(Vector3 position) const
{
	return Ray(position, -direction + Vector3::random_in_unit_sphere()*0.5f);
}

float DirectionalLight::get_distance(Vector3 position) const
{
	return FLT_MAX;
}
