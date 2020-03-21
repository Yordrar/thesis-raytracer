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

	inline void set_normals(Vector3 n_v0, Vector3 n_v1, Vector3 n_v2) {
		normal_v0 = n_v0;
		normal_v1 = n_v1;
		normal_v2 = n_v2;
	}

	// Intersectable interface
	Hit get_intersection(const Ray& ray) const override;
	AxisAlignedBoundingBox get_bounding_box() const override;

private:
	Vector3 v0, v1, v2;
	Vector3 edge01, edge02;
	Vector3 normal_v0, normal_v1, normal_v2;
	Vector3 normal_flat;

	inline Vector3 get_normal_smooth(float u, float v, float w) const {
		return (u*normal_v1 + v*normal_v2 + w*normal_v0).unit();
	}
};

