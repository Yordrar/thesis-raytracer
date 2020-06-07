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
	Vector3 light_vector = this->position - _position;
    return (albedo * (intensity / light_vector.get_squared_magnitude())).clamp(0, 1.0f);
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
