#include "RandomAxis.h"

RandomAxis::RandomAxis()
{

}

RandomAxis::~RandomAxis()
{

}

BVHBuildStrategy::SPLIT_AXIS RandomAxis::get_split_axis(std::vector<Intersectable *> &intersectables) const
{
    int axis = static_cast<int>(Math::Randf()*3.0f);

    switch(axis) {
    case 0:
        return SPLIT_AXIS::X_AXIS;
    case 1:
        return SPLIT_AXIS::Y_AXIS;
    case 2:
        return SPLIT_AXIS::Z_AXIS;
    }
}
