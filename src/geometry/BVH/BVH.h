#pragma once

#include <vector>

#include <geometry/Intersectable.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class BVH : Intersectable
{
public:
	BVH();
	BVH(std::vector<Intersectable*> intersectables, int num_elem);
	~BVH() override;

	std::pair<const Intersectable*, float> get_intersection(Ray ray) const override;

private:
	Intersectable* left;
	Intersectable* right;

	// Intersectable interface
public:
	Vector3 get_normal(Vector3 point) const override;

	// Intersectable interface
public:
	AxisAlignedBoundingBox get_bounding_box() const override;
};

