#include "Triangle.h"

Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	: v0(v0),
	  v1(v1),
	  v2(v2)
{
	hasUV = false;

	edge01 = v1 - v0;
	edge02 = v2 - v0;
	normal_flat = edge01.cross(edge02).unit();

	Vector3 min_corner = v0.min(v1).min(v2);
	Vector3 max_corner = v0.max(v1).max(v2);
	bounding_box = AxisAlignedBoundingBox(min_corner, max_corner);
}

Triangle::~Triangle()
{

}

Vector3 create_not_collinear(Vector3 w) {
	Vector3 t = w;
	float t_x = fabsf(t.get_x());
	float t_y = fabsf(t.get_y());
	float t_z = fabsf(t.get_z());
	if(t_x <= t_y && t_x <= t_z) {
		t.set_x(1);
	}
	else if(t_y <= t_x && t_y <= t_z) {
		t.set_y(1);
	}
	else if(t_z <= t_y && t_z <= t_x) {
		t.set_z(1);
	}
	return t;
}

void Triangle::generate_tangents()
{
	if(hasNormals) {
		Vector3 t = create_not_collinear(normal_v0);
		tangent_v0 = t.cross(normal_v0).unit();
		bitangent_v0 = normal_v0.cross(tangent_v0).unit();

		t = create_not_collinear(normal_v1);
		tangent_v1 = t.cross(normal_v1).unit();
		bitangent_v1 = normal_v1.cross(tangent_v1).unit();

		t = create_not_collinear(normal_v2);
		tangent_v2 = t.cross(normal_v2).unit();
		bitangent_v2 = normal_v2.cross(tangent_v2).unit();

		hasTangents = true;
	}
}

Hit Triangle::get_intersection(const Ray& ray) const
{
	// Möller-Trumbore algorithm
	Vector3 pvec, tvec, qvec;
	float det, invDet, u, v;
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
		 // There is an intersection
		Hit intersection(true, nullptr, get_normal(u, v, 1-u-v), t);
		if(hasUV) {
			intersection.set_uv(u*uv_v1 + v*uv_v2 + (1-u-v)*uv_v0);
		}
		if(hasTangents) {
			intersection.set_tangent(u*tangent_v1 + v*tangent_v2 + (1-u-v)*tangent_v0);
			intersection.set_bitangent(u*bitangent_v1 + v*bitangent_v2 + (1-u-v)*bitangent_v0);
		}
		return intersection;
	}
	else // The ray is contained in the triangle
		return Hit();
}

AxisAlignedBoundingBox Triangle::get_bounding_box() const
{
	return bounding_box;
}
