#pragma once

#include "BVHBuildStrategy.h"

class SurfaceAreaHeuristic : public BVHBuildStrategy
{
public:
    SurfaceAreaHeuristic();
    ~SurfaceAreaHeuristic();

	std::pair<int, SPLIT_AXIS> get_split_axis(std::vector<Intersectable*>& intersectables) const override;
};
