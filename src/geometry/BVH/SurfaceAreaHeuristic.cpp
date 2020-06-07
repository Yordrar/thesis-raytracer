#include "SurfaceAreaHeuristic.h"

SurfaceAreaHeuristic::SurfaceAreaHeuristic()
{

}

SurfaceAreaHeuristic::~SurfaceAreaHeuristic()
{

}

BVHBuildStrategy::SPLIT_AXIS SurfaceAreaHeuristic::get_split_axis(std::vector<Intersectable*>& intersectables) const
{
    std::vector<SPLIT_AXIS> axis{SPLIT_AXIS::X_AXIS,
                                 SPLIT_AXIS::Y_AXIS,
                                 SPLIT_AXIS::Z_AXIS};

    SPLIT_AXIS best_axis;
    for(SPLIT_AXIS axis : axis) {
        size_t num_elem = intersectables.size();
        switch (axis) {
        case SPLIT_AXIS::X_AXIS:
            std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), AxisAlignedBoundingBox::box_x_compare);
            break;
        case SPLIT_AXIS::Y_AXIS:
            std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), AxisAlignedBoundingBox::box_y_compare);
            break;
        case SPLIT_AXIS::Z_AXIS:
            std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), AxisAlignedBoundingBox::box_z_compare);
            break;
        }

        return SPLIT_AXIS::X_AXIS;
    }
}
