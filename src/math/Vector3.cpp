#include "Vector3.h"

#include <cmath>

Vector3::Vector3()
	: x(0),
	  y(0),
	  z(0)
{

}

Vector3::Vector3(float val)
	: x(val),
	  y(val),
	  z(val)
{

}

Vector3::Vector3(float x, float y, float z)
	: x(x),
	  y(y),
	  z(z)
{

}

Vector3 Vector3::unit() const
{
	auto magnitude = this->get_magnitude();
	if(magnitude == 0.0f) {
		return *this;
	}
	else {
		return *this / magnitude;
	}
}

Vector3 Vector3::create_not_collinear() const
{
	Vector3 t = *this;
	float t_x = fabsf(this->get_x());
	float t_y = fabsf(this->get_y());
	float t_z = fabsf(this->get_z());
	if(t_x <= t_y && t_x <= t_z) {
		t.set_x(1);
	}
	else if(t_y <= t_x && t_y <= t_z) {
		t.set_y(1);
	}
	else if(t_z <= t_y && t_z <= t_x) {
		t.set_z(1);
	}
	return t;
}

Vector3 Vector3::random_in_unit_sphere()
{
	Vector3 p;
	do {
		p = 2.0f*Vector3(Math::Randf(), Math::Randf(), Math::Randf()) - Vector3(1, 1, 1);
	} while (p.get_magnitude() > 1.0f);
	return p;
}
