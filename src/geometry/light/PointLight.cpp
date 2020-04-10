#include "PointLight.h"

PointLight::PointLight(Vector3 _albedo, float _intensity)
	: Emitter(_albedo)
{
	intensity = _intensity;
}

PointLight::~PointLight()
{

}

Vector3 PointLight::get_emission_color(Vector3 _position, Vector3 normal) const
{
	Vector3 light_vector = this->position - _position;
	return albedo * intensity / light_vector.get_squared_magnitude() * light_vector.unit().dot(normal);
}

Ray PointLight::get_shadow_ray(Vector3 _position) const
{
	Vector3 shadow_ray_dir = this->position - _position;
	return Ray(_position, shadow_ray_dir);
}

float PointLight::get_distance(Vector3 _position) const
{
	return (this->position - _position).get_magnitude();
}
