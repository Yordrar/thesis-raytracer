#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

#include <geometry/Intersectable.h>

#include <geometry/BVH/AxisAlignedBoundingBox.h>

class Triangle : public Intersectable
{
public:
	Triangle(Vector3 v0, Vector3 v1, Vector3 v2);
	~Triangle() override;

	// Intersectable interface
	Vector3 get_normal(Vector3 point) const override;
	std::pair<const Intersectable*, float> get_intersection(Ray ray) const override;
	AxisAlignedBoundingBox get_bounding_box() const override;

private:
	Vector3 v0, v1, v2;
	Vector3 edge01, edge02;
	Vector3 normal;
};

