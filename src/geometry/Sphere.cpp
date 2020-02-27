#include "Sphere.h"

Sphere::Sphere(Vector3 _position, float radius)
	: Entity(_position),
	  radius(radius)
{

}

Sphere::~Sphere()
{

}

float Sphere::get_intersection(Ray r) const
{
	Vector3 oc = r.get_origin() - position;
	float a = r.get_direction().dot(r.get_direction());
	float b = oc.dot(r.get_direction());
	float c = oc.dot(oc) - radius*radius;
	float discriminant = b*b - a*c;
	float t1 = -b + sqrtf(discriminant) / a;
	float t2 = -b - sqrtf(discriminant) / a;
	if (discriminant > 0) {
		if(t1 > 0 && t2 > 0) {
			return fminf(t1, t2);
		}
		if (t2 < FLT_MAX && t2 > 0) {
			return t2;
		}
		if (t1 < FLT_MAX && t1 > 0) {
			return t1;
		}
	}
	return -1.0f;
}

Vector3 Sphere::get_normal(Vector3 point) const {
	return (point - position).unit();
}
