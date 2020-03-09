#include "Triangle.h"

Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2)
	: v0(v0),
	  v1(v1),
	  v2(v2)
{
	edge01 = v1 - v0;
	edge02 = v2 - v0;
	normal = edge01.cross(edge02).unit();
}

float Triangle::get_intersection(Ray ray) const
{
	// Möller-Trumbore algorithm
	Vector3 h, s, q;
	float a,f,u,v;
	h = ray.get_direction().cross(edge02);
	a = edge01.dot(h);
	if(Math::Float_Eq(a, 0.0f))
		return 0.0f; // This ray is parallel to this triangle.
	f = 1.0f/a;
	s = ray.get_origin() - v0;
	u = f * s.dot(h);
	if (u < 0.0f || u > 1.0f)
		return 0.0f;
	q = s.cross(edge01);
	v = f * ray.get_direction().dot(q);
	if (v < 0.0f || u + v > 1.0f)
		return 0.0f;
	float t = f * edge02.dot(q);
	if (!Math::Float_Eq(t, 0.0f)) {
		return t; // There is an intersection
	}
	else // The ray is contained in the triangle
		return 0.0f;
}

Vector3 Triangle::get_normal() const
{
	return normal;
}
