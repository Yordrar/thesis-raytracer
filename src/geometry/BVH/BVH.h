#pragma once

#include <vector>

#include <geometry/Intersectable.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class BVH : public Intersectable
{
public:
	BVH();
	BVH(std::vector<Intersectable*> intersectables, int num_elem);
	~BVH() override;

	Hit get_intersection(Ray ray) const override;

private:
	Intersectable* left;
	Intersectable* right;
};

