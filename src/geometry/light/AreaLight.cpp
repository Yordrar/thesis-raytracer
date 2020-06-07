#include "AreaLight.h"

AreaLight::AreaLight(Vector3 albedo, float intensity)
	: Emitter(albedo),
	  intensity(intensity)
{
	right = Vector3(1, 0, 0);
	up = Vector3(0, 1, 0);
}

AreaLight::~AreaLight()
{

}

void AreaLight::rotate_global(float euler_x, float euler_y, float euler_z)
{
	Entity::rotate_global(euler_x, euler_y, euler_z);
	Quaternion qx = Quaternion::create_rotation(euler_x, Vector3(1, 0, 0));
	Quaternion qy = Quaternion::create_rotation(euler_y, Vector3(0, 1, 0));
	Quaternion qz = Quaternion::create_rotation(euler_z, Vector3(0, 0, 1));
	Quaternion q = qz*(qy*qx).unit();
	up = q.apply(up);
	right = q.apply(right);
}

Vector3 AreaLight::get_emission_color(Vector3 _position) const
{
    Vector3 light_vector = this->position - _position;
    return (albedo * (intensity / light_vector.get_squared_magnitude())).clamp(0, 1.0f);
}

Ray AreaLight::get_shadow_ray(Vector3 position) const
{
	Vector3 point = this->position + up*(Math::Randf()*2*size - size) + right*(Math::Randf()*2*size - size);
	return Ray(position, point - position);
}

float AreaLight::get_distance(Vector3 position) const
{
	return (this->position - position).get_magnitude();
}
