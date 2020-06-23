#pragma once

#include "BVHBuildStrategy.h"

class RandomAxis : public BVHBuildStrategy
{
public:
    RandomAxis();
    ~RandomAxis();

	std::pair<int, SPLIT_AXIS> get_split_axis(std::vector<Intersectable *> &intersectables) const override;
};
