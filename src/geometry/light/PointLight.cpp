#include "PointLight.h"

PointLight::PointLight(Vector3 _albedo, float _intensity)
	: Emitter(_albedo, _intensity)
{

}

PointLight::~PointLight()
{

}

Vector3 PointLight::get_emission_color(Vector3 _position) const
{
	return albedo * intensity / (this->position - _position).get_squared_magnitude();
}
