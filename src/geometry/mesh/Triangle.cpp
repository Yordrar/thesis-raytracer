#include "Triangle.h"

Triangle::Triangle(Vector3 v0, Vector3 v1, Vector3 v2)
	: v0(v0),
	  v1(v1),
	  v2(v2)
{
	edge01 = v1 - v0;
	edge02 = v2 - v0;
	normal = edge02.cross(edge01).unit();

	Vector3 min_corner = v0.min(v1).min(v2);
	Vector3 max_corner = v0.max(v1).max(v2);
	bounding_box = AxisAlignedBoundingBox(min_corner, max_corner);
}

Triangle::~Triangle()
{

}

Hit Triangle::get_intersection(Ray ray) const
{
	// Möller-Trumbore algorithm
	Vector3 pvec, tvec, qvec;
	float det,invDet,u,v;
	pvec = ray.get_direction().cross(edge02);
	det = edge01.dot(pvec);
	if(Math::Float_Eq(det, 0.0f))
		return Hit(); // This ray is parallel to this triangle.
	invDet = 1.0f/det;
	tvec = ray.get_origin() - v0;
	u = invDet * tvec.dot(pvec);
	if (u < 0.0f || u > 1.0f)
		return Hit();
	qvec = tvec.cross(edge01);
	v = invDet * ray.get_direction().dot(qvec);
	if (v < 0.0f || u + v > 1.0f)
		return Hit();
	float t = invDet * edge02.dot(qvec);
	if (t > 0.001f) {
		return Hit(true, nullptr, normal, t); // There is an intersection
	}
	else // The ray is contained in the triangle
		return Hit();
}


AxisAlignedBoundingBox Triangle::get_bounding_box() const
{
	return bounding_box;
}
