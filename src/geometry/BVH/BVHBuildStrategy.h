#pragma once

#include <geometry/Intersectable.h>

class BVHBuildStrategy
{
protected:
    BVHBuildStrategy();
    virtual ~BVHBuildStrategy();

public:
    enum class SPLIT_AXIS {
        X_AXIS,
        Y_AXIS,
        Z_AXIS
    };

    virtual SPLIT_AXIS get_split_axis(std::vector<Intersectable*>& intersectables) const = 0;
};
