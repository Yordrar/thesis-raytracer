#include "RandomAxis.h"

RandomAxis::RandomAxis()
{

}

RandomAxis::~RandomAxis()
{

}

std::pair<int, BVHBuildStrategy::SPLIT_AXIS> RandomAxis::get_split_axis(std::vector<Intersectable *> &intersectables) const
{
    int axis = static_cast<int>(Math::Randf()*3.0f);

    switch(axis) {
    case 0:
		return std::make_pair(static_cast<int>(intersectables.size())/2, SPLIT_AXIS::X_AXIS);
	case 1:
		return std::make_pair(static_cast<int>(intersectables.size())/2, SPLIT_AXIS::Y_AXIS);
	case 2:
		return std::make_pair(static_cast<int>(intersectables.size())/2, SPLIT_AXIS::Z_AXIS);
    }
}
