#pragma once

#include <vector>

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class BVH : public Intersectable
{
public:
	BVH();
	BVH(std::vector<Intersectable*> intersectables);
	~BVH() override;

	Hit get_intersection(const Ray& ray) const override;
	Entity* get_intersectable(const Ray& ray) const;

	int count() const;

private:
	Intersectable* left;
	Intersectable* right;
};

