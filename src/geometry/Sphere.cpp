#include "Sphere.h"

Sphere::Sphere(Vector3 _position, float radius)
	: Entity(_position),
	  Intersectable(),
	  radius(radius)
{
	Vector3 radius_vec(radius, radius, radius);
	bounding_box = AxisAlignedBoundingBox(position - radius_vec, position + radius_vec);
}

Sphere::~Sphere()
{

}

#define T_MIN 0.001f
std::pair<const Intersectable*, float> Sphere::get_intersection(Ray r) const
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
			return std::make_pair<const Intersectable*, float>(this, fminf(t1, t2));
		}
		if (t2 < FLT_MAX && t2 > T_MIN) {
			return std::make_pair<const Intersectable*, float>(this, static_cast<float>(t2));
		}
		if (t1 < FLT_MAX && t1 > T_MIN) {
			return std::make_pair<const Intersectable*, float>(this, static_cast<float>(t1));
		}
	}
	return std::make_pair<Intersectable*, float>(nullptr, 0.0f);
}


Vector3 Sphere::get_normal(Vector3 point) const
{
	return (point - position) / radius;
}

AxisAlignedBoundingBox Sphere::get_bounding_box() const
{
	return bounding_box;
}
