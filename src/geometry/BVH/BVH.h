#pragma once

#include <vector>

#include <geometry/Intersectable.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class BVH : public Intersectable
{
public:
	BVH();
	BVH(std::vector<Intersectable*> intersectables);
	~BVH() override;

	Hit get_intersection(Ray ray) const override;

	int count() const;

private:
	Intersectable* left;
	Intersectable* right;
};

