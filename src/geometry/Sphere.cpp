#include "Sphere.h"

Sphere::Sphere(const Vector3& _position, float radius)
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

Vector3 get_uv(Vector3 normal) {
	auto phi = atan2f(normal.get_z(), normal.get_x());
	auto theta = asinf(normal.get_y());
	return Vector3(1-(phi + Math::PI) / (2*Math::PI), (theta + Math::PI/2) / Math::PI, 0);
}

#define T_MIN 0.001f
Hit Sphere::get_intersection(const Ray& r) const
{
	Vector3 oc = r.get_origin() - position;
	float a = r.get_direction().dot(r.get_direction());
	float b = oc.dot(r.get_direction());
	float c = oc.dot(oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0.0f) {
		float t1 = -b + sqrtf(discriminant) / a;
		float t2 = -b - sqrtf(discriminant) / a;
		float t_min;
		if(t1 > T_MIN && t2 > T_MIN) {
			t_min = Math::Fast_Min(t1, t2);
		}
		else if (t2 < FLT_MAX && t2 > T_MIN) {
			t_min = t2;
		}
		else if (t1 < FLT_MAX && t1 > T_MIN) {
			t_min = t1;
		}
		else {
			return Hit();
		}
		Vector3 normal = get_normal(r.get_point(t_min));
		return Hit(true, material, normal, t_min, get_uv(normal));
	}
	return Hit();
}


Vector3 Sphere::get_normal(const Vector3& point) const
{
	return (point - position).unit();
}
