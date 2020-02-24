#include "Vector3.h"

#include <cmath>

Vector3::Vector3()
{
	x = y = z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
