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

Vector3 Vector3::random_in_unit_sphere()
{
	Vector3 p;
	do {
		p = 2.0f*Vector3(Math::Randf(), Math::Randf(), Math::Randf()) - Vector3(1, 1, 1);
	} while (p.get_magnitude() > 1.0f);
	return p;
}
