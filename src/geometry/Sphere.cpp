#include "Sphere.h"

Sphere::Sphere(Vector3 _position, float radius)
	: Entity(_position),
	  Intersectable(),
	  radius(radius)
{

}

Sphere::~Sphere()
{

}

#define T_MIN 0.001f
float Sphere::get_intersection(Ray r) const
{
	Vector3 oc = r.get_origin() - position;
	float a = r.get_direction().dot(r.get_direction());
	float b = oc.dot(r.get_direction());
	float c = oc.dot(oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float t1 = -b + sqrtf(discriminant) / a;
		float t2 = -b - sqrtf(discriminant) / a;
		if(t1 > T_MIN && t2 > T_MIN) {
			return fminf(t1, t2);
		}
		if (t2 < FLT_MAX && t2 > T_MIN) {
			return t2;
		}
		if (t1 < FLT_MAX && t1 > T_MIN) {
			return t1;
		}
	}
	return -1.0f;
}

Vector3 Sphere::get_normal(Vector3 point) const {
	return (point - position) / radius;
}

Ray Sphere::scatter(Ray ray, float t) const
{
	return material->scatter(ray, t, get_normal(ray.get_point(t)));
}
