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

Quaternion::Quaternion(float real, Vector3 imaginary)
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

Vector3 Quaternion::apply(Vector3 point) const
{
	Quaternion p(0, point);
	return (*this * p * get_inverse()).get_imaginary();
}

Quaternion Quaternion::slerp(Quaternion other, float t) const
{
	float cos_angle = this->unit().get_imaginary().dot(other.unit().get_imaginary());
	float angle = acosf(cos_angle);
	float sin_angle = sinf(angle);
	return (*this)*(sinf((1-t)*angle)/sin_angle) + other*(sinf(t*angle)/sin_angle);
}

Quaternion Quaternion::operator*(Quaternion other) const
{
	Vector3 this_imaginary(x, y, z);
	Vector3 other_imaginary = other.get_imaginary();
	return Quaternion(w*other_imaginary + other.w*this_imaginary + this_imaginary.cross(other_imaginary),
					  w*other.w - this_imaginary.dot(other_imaginary));
}
