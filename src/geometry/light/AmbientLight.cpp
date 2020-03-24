#include "AmbientLight.h"

AmbientLight::AmbientLight(Vector3 _albedo, Vector3 direction)
	: Emitter(_albedo)
{
	this->direction = direction;
}

AmbientLight::~AmbientLight()
{

}

Vector3 AmbientLight::get_emission_color(Vector3 _position) const
{
	return albedo;
}

Ray AmbientLight::get_shadow_ray(Vector3 position) const
{
	return Ray(position, -direction + Vector3::random_in_unit_sphere());
}

float AmbientLight::get_distance(Vector3 position) const
{
	return FLT_MAX;
}
