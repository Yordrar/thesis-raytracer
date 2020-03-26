#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Vector3 _albedo, Vector3 direction)
	: Emitter(_albedo)
{
	this->direction = direction;
}

DirectionalLight::~DirectionalLight()
{

}

Vector3 DirectionalLight::get_emission_color(Vector3 _position, Vector3 normal) const
{
	return albedo * normal.dot(-direction);
}

Ray DirectionalLight::get_shadow_ray(Vector3 position) const
{
	return Ray(position, -direction);
}

float DirectionalLight::get_distance(Vector3 position) const
{
	return FLT_MAX;
}
