#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

#include <geometry/Intersectable.h>
#include <geometry/Scatterer.h>

#include <geometry/BVH/AxisAlignedBoundingBox.h>

class Triangle : public Intersectable
{
public:
	Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);
	~Triangle() override;

	inline void set_vertices(Vector3 vertex0, Vector3 vertex1, Vector3 vertex2) {
		v0 = vertex0;
		v1 = vertex1;
		v2 = vertex2;
	}

	inline void set_normals(Vector3 n_vertex0, Vector3 n_vertex1, Vector3 n_vertex2) {
		normal_v0 = n_vertex0;
		normal_v1 = n_vertex1;
		normal_v2 = n_vertex2;
	}

	inline void set_uv_coords(Vector3 uv_vertex0, Vector3 uv_vertex1, Vector3 uv_vertex2) {
		hasUV = true;
		uv_v0 = uv_vertex0;
		uv_v1 = uv_vertex1;
		uv_v2 = uv_vertex2;
	}

	// Intersectable interface
	Hit get_intersection(const Ray& ray) const override;
	AxisAlignedBoundingBox get_bounding_box() const override;

private:
	Vector3 v0, v1, v2;
	Vector3 edge01, edge02;
	Vector3 normal_v0, normal_v1, normal_v2;
	Vector3 normal_flat;
	bool hasUV;
	Vector3 uv_v0, uv_v1, uv_v2;

	inline Vector3 get_normal_smooth(float u, float v, float w) const {
		return (u*normal_v1 + v*normal_v2 + w*normal_v0).unit();
	}
};

