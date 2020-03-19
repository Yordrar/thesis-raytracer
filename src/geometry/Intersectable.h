#pragma once

#include <geometry/BVH/AxisAlignedBoundingBox.h>

#include <geometry/Hit.h>

class Intersectable
{
protected:
	Intersectable();
	virtual ~Intersectable() = 0;

	AxisAlignedBoundingBox bounding_box;

public:
	virtual Hit get_intersection(const Ray& ray) const = 0;
	virtual AxisAlignedBoundingBox get_bounding_box() const {return bounding_box;}
};

