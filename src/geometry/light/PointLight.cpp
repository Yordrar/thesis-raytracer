#include "PointLight.h"

PointLight::PointLight(Vector3 _albedo, float _intensity)
	: Emitter(_albedo)
{
	intensity = _intensity;
}

PointLight::~PointLight()
{

}

Vector3 PointLight::get_emission_color(Vector3 _position) const
{
	return albedo * intensity / (this->position - _position).get_squared_magnitude();
}

Ray PointLight::get_shadow_ray(Vector3 _position) const
{
	Vector3 shadow_ray_dir = this->position - _position + Vector3::random_in_unit_sphere();
	Ray shadow_ray(_position, shadow_ray_dir);
}

float PointLight::get_distance(Vector3 _position) const
{
	return (this->position - _position).get_magnitude();
}
