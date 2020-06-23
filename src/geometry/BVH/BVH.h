#pragma once

#include "BVHBuildStrategy.h"

#include <vector>

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class BVH : public Intersectable
{
public:
	BVH(std::vector<Intersectable*> intersectables, BVHBuildStrategy* strategy);
	~BVH() override;

	Hit get_intersection(const Ray& ray) const override;
	Entity* get_intersectable(const Ray& ray) const;

    int get_num_nodes() const;
    int get_num_intersectables() const;

private:
	Intersectable* left;
	Intersectable* right;
};

