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

	// Intersectable interface
	Hit get_intersection(const Ray& ray) const override;
	AxisAlignedBoundingBox get_bounding_box() const override;

private:
	Vector3 v0, v1, v2;
	Vector3 edge01, edge02;
	Vector3 normal;
};

