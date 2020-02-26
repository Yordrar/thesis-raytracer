#include "Sphere.h"

Sphere::Sphere(Vector3 _position, float radius)
	: Entity(_position),
	  radius(radius)
{

}

Sphere::~Sphere()
{

}

float Sphere::intersects(Ray r) const
{
	Vector3 oc = r.get_origin() - position;
	float a = r.get_direction().dot(r.get_direction());
	float b = 2.0f * oc.dot(r.get_direction());
	float c = oc.dot(oc) - radius*radius;
	float discriminant = b*b - 4.0f*a*c;
	float t1 = -b + sqrtf(discriminant) / 2.0f*a;
	float t2 = -b - sqrtf(discriminant) / 2.0f*a;
	if (discriminant > 0) {
		if (t2 < FLT_MAX && t2 > 1) {
			return t2;
		}
		if (t1 < FLT_MAX && t1 > 1) {
			return t1;
		}
	}
	return -1.0f;
}

Vector3 Sphere::get_normal(Vector3 point) const {
	return (point - position).unit();
}
