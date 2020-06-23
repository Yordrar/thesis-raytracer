#include "RandomAxis.h"
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

	float area = 0;
	for(Intersectable* i : intersectables) {
		area += i->get_bounding_box().get_surface_area();
	}
	return area;
}

std::pair<int, BVHBuildStrategy::SPLIT_AXIS> SurfaceAreaHeuristic::get_split_axis(std::vector<Intersectable*>& intersectables) const
{
	if(intersectables.size() <= 4) return RandomAxis().get_split_axis(intersectables);

	std::vector<SPLIT_AXIS> all_axis{SPLIT_AXIS::X_AXIS,
									 SPLIT_AXIS::Y_AXIS,
									 SPLIT_AXIS::Z_AXIS};
	size_t num_elem = intersectables.size();
	float total_area = get_area(intersectables);


	std::vector<Areas> areas(intersectables.size());

	bool is_there_best_axis = false;
    SPLIT_AXIS best_axis;
	float best_cost = intersectables.size();
	int best_event = -1;
	for(SPLIT_AXIS axis : all_axis) {
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

		{
			// Sweep from left
			std::vector<Intersectable*> s1(0);
			std::vector<Intersectable*> s2 = intersectables;
			for(int i = 0; i < static_cast<int>(intersectables.size()); i++) {
				areas[i].left_area = get_area(s1);
				s1.push_back(s2[i]);
			}
		}

		{
			// Sweep from right
			std::vector<Intersectable*> s1 = intersectables;
			std::vector<Intersectable*> s2(0);
			for(int i = intersectables.size()-1; i >= 0; i--) {
				areas[i].right_area = get_area(s2);
				float this_cost = (areas[i].left_area / total_area) * s1.size() + (areas[i].right_area / total_area) * s2.size();
				s2.push_back(s1[i]);
				s1.pop_back();
				if(this_cost < best_cost) {
					best_cost = this_cost;
					best_event = i;
					best_axis = axis;
					is_there_best_axis = true;
				}
			}
		}
    }

	if(!is_there_best_axis) {
		return RandomAxis().get_split_axis(intersectables);
	}
	else {
		return std::make_pair(best_event, best_axis);
	}
}
