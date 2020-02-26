#include "Vector3.h"

#include <cmath>

Vector3::Vector3()
	: x(0),
	  y(0),
	  z(0)
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
