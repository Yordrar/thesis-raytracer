#include "Triangle.h"

#include <material/Lambertian.h>

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

void Triangle::generate_tangents()
{
	if(hasNormals) {
		Vector3 t = normal_v0.create_not_collinear();
		tangent_v0 = t.cross(normal_v0).unit();
		bitangent_v0 = normal_v0.cross(tangent_v0).unit();

		t = normal_v1.create_not_collinear();
		tangent_v1 = t.cross(normal_v1).unit();
		bitangent_v1 = normal_v1.cross(tangent_v1).unit();

		t = normal_v2.create_not_collinear();
		tangent_v2 = t.cross(normal_v2).unit();
		bitangent_v2 = normal_v2.cross(tangent_v2).unit();

		hasTangents = true;
	}
}

void Triangle::translate(float delta_x, float delta_y, float delta_z)
{
	Vector3 delta(delta_x, delta_y, delta_z);
	v0 += delta;
	v1 += delta;
	v2 += delta;

	Vector3 min_corner = v0.min(v1).min(v2);
	Vector3 max_corner = v0.max(v1).max(v2);
	bounding_box = AxisAlignedBoundingBox(min_corner, max_corner);
}

void Triangle::rotate_vertices(Quaternion rotation)
{
	v0 = rotation.apply(v0);
	v1 = rotation.apply(v1);
	v2 = rotation.apply(v2);
	normal_v0 = rotation.apply(normal_v0);
	normal_v1 = rotation.apply(normal_v1);
	normal_v2 = rotation.apply(normal_v2);
	edge01 = v1 - v0;
	edge02 = v2 - v0;
	normal_flat = edge01.cross(edge02).unit();

	Vector3 min_corner = v0.min(v1).min(v2);
	Vector3 max_corner = v0.max(v1).max(v2);
	bounding_box = AxisAlignedBoundingBox(min_corner, max_corner);
	generate_tangents();
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
		intersection.set_material(new Lambertian);
		intersection.set_entity_position((get_vertex0() + get_vertex1() + get_vertex2()) / 3.0f);
		return intersection;
	}
	else // The ray is contained in the triangle
		return Hit();
}

AxisAlignedBoundingBox Triangle::get_bounding_box() const
{
	return bounding_box;
}
