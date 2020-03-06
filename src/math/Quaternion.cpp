#include "Quaternion.h"


Quaternion::Quaternion(float x, float y, float z, float w)
	: x(x),
	  y(y),
	  z(z),
	  w(w)
{

}

Quaternion::Quaternion(Vector3 imaginary, float real)
	: x(imaginary.get_x()),
	  y(imaginary.get_y()),
	  z(imaginary.get_z()),
	  w(real)
{

}

Quaternion Quaternion::get_inverse() const
{
	Vector3 imaginary = get_imaginary();
	float squared_magnitude = get_squared_magnitude();
	return Quaternion(-imaginary/squared_magnitude, w/squared_magnitude);
}

Quaternion Quaternion::operator*(Quaternion other) const
{
	Vector3 this_imaginary(x, y, z);
	Vector3 other_imaginary = other.get_imaginary();
	return Quaternion(w*other_imaginary + other.w*this_imaginary + this_imaginary.cross(other_imaginary),
					  w*other.w - this_imaginary.dot(other_imaginary));
}
