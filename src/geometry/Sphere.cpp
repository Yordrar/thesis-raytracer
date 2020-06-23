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

AxisAlignedBoundingBox Sphere::get_bounding_box() const
{
	Vector3 radius_vec(radius, radius, radius);
	return AxisAlignedBoundingBox(position - radius_vec, position + radius_vec);
}

Vector3 get_uv(Vector3 normal) {
	auto phi = atan2f(normal.get_z(), normal.get_x());
	auto theta = asinf(normal.get_y());
	return Vector3(1.0f-(phi + Math::PI) / (2.0f*Math::PI), (theta + Math::PI/2.0f) / Math::PI, 0);
}

Vector3 generate_tangent(Vector3 normal)
{
	Vector3 t = normal.create_not_collinear();
	return t.cross(normal).unit();
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
		Vector3 tangent = generate_tangent(normal);
		return Hit(true, material, normal, t_min, get_uv(normal), tangent, normal.cross(tangent).unit(), position);
	}
	return Hit();
}


Vector3 Sphere::get_normal(const Vector3& point) const
{
	return (point - position).unit();
}
