#include "SurfaceAreaHeuristic.h"

SurfaceAreaHeuristic::SurfaceAreaHeuristic()
{

}

SurfaceAreaHeuristic::~SurfaceAreaHeuristic()
{

}

struct Areas {
	float left_area;
	float right_area;
};

float get_area(std::vector<Intersectable*> intersectables) {
	if(intersectables.size() == 0) return FLT_MAX;

	float area;
	for(Intersectable* i : intersectables) {
		area += i->get_bounding_box().get_surface_area();
	}
	return area;
}

BVHBuildStrategy::SPLIT_AXIS SurfaceAreaHeuristic::get_split_axis(std::vector<Intersectable*>& intersectables) const
{
    std::vector<SPLIT_AXIS> axis{SPLIT_AXIS::X_AXIS,
                                 SPLIT_AXIS::Y_AXIS,
                                 SPLIT_AXIS::Z_AXIS};


	std::vector<Areas> areas(intersectables.size());

	bool is_there_best_axis = false;
    SPLIT_AXIS best_axis;
	float best_cost = intersectables.size();
	std::vector<Intersectable*>::size_type best_event = -1;
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

		// Sweep from left
		std::vector<Intersectable*> s1(intersectables.size());
		std::vector<Intersectable*> s2 = intersectables;
		for(std::vector<Intersectable*>::size_type i = 0; i < intersectables.size(); i++) {
			areas[i].left_area = get_area(s1);
			s1[i] = s2[i];
			s2.begin() = s2.begin()+1;
		}

		// Sweep from right
		s1 = intersectables;
		s2 = std::vector<Intersectable*>(intersectables.size());
		for(std::vector<Intersectable*>::size_type i = intersectables.size(); i >= intersectables.size(); i--) {
			areas[i].right_area = get_area(s2);
			// evaluate equation 2
			float this_cost = 0; // CALCULATE
			s2[i] = s1[i];
			s1.pop_back();
			if(this_cost < best_cost) {
				best_cost = this_cost;
				best_event = i;
				best_axis = axis;
				is_there_best_axis = true;
			}
		}
    }

	if(!is_there_best_axis) {
		return SPLIT_AXIS::X_AXIS;
	}
	else {

	}
}
