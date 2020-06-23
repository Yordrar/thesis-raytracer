#include "BVH.h"
#include "RandomAxis.h"
#include "SurfaceAreaHeuristic.h"

#include <algorithm>
#include <iostream>

BVH::BVH(std::vector<Intersectable*> intersectables, BVHBuildStrategy* strategy)
{
	int num_elem = static_cast<int>(intersectables.size());
    if (num_elem == 1) {
		left = intersectables[0];
		right = intersectables[0];
		bounding_box = intersectables[0]->get_bounding_box();
		return;
	}
	else if (num_elem == 2) {
		left = intersectables[0];
		right = intersectables[1];
	}
    else {
		std::pair<int, BVHBuildStrategy::SPLIT_AXIS> axis = strategy->get_split_axis(intersectables);

		switch(axis.second) {
        case BVHBuildStrategy::SPLIT_AXIS::X_AXIS:
            std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), AxisAlignedBoundingBox::box_x_compare);
            break;
        case BVHBuildStrategy::SPLIT_AXIS::Y_AXIS:
            std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), AxisAlignedBoundingBox::box_y_compare);
            break;
        case BVHBuildStrategy::SPLIT_AXIS::Z_AXIS:
            std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), AxisAlignedBoundingBox::box_z_compare);
            break;
		}
		auto half = intersectables.begin() + axis.first;

		std::vector<Intersectable*> left_half(intersectables.begin(), half);
		left = new BVH(left_half, strategy);

		std::vector<Intersectable*> right_half(half, intersectables.end());
		right = new BVH(right_half, strategy);
	}

	AxisAlignedBoundingBox box_left = left->get_bounding_box();
	AxisAlignedBoundingBox box_right = right->get_bounding_box();

	bounding_box = AxisAlignedBoundingBox::surrounding_box(box_left, box_right);
}

BVH::~BVH()
{
	BVH* l = dynamic_cast<BVH*>(left);
	BVH* r = dynamic_cast<BVH*>(right);

	if(l) delete l;
	if(r) delete r;
}

Hit BVH::get_intersection(const Ray& ray) const
{
	if(bounding_box.hit(ray, FLT_MIN, FLT_MAX)) {
		if(left == right) {
			return left->get_intersection(ray);
		}

		Hit hit_left = left->get_intersection(ray);
		if (!hit_left.is_hit()) {
			return right->get_intersection(ray);
		}

		Hit hit_right = right->get_intersection(ray);
		if(hit_right.is_hit() && hit_right.get_t() < hit_left.get_t()) {
			return hit_right;
		}
		return hit_left;
	}
	return Hit();
}

Entity* BVH::get_intersectable(const Ray& ray) const
{
	if(bounding_box.hit(ray, FLT_MIN, FLT_MAX)) {
		Entity* left_entity = dynamic_cast<Entity*>(left);
		Entity* right_entity = dynamic_cast<Entity*>(right);
		Hit hit_left = left->get_intersection(ray);
		Hit hit_right = right->get_intersection(ray);

		if(hit_left.is_hit() && (hit_left.get_t() <= hit_right.get_t() || !hit_right.is_hit())) {
			if(left_entity) {
				return left_entity;
			}
			return dynamic_cast<BVH*>(left)->get_intersectable(ray);
		}

		if(hit_right.is_hit() && (hit_right.get_t() <= hit_left.get_t() || !hit_left.is_hit())) {
			if(right_entity) {
				return right_entity;
			}
			return dynamic_cast<BVH*>(right)->get_intersectable(ray);
		}
		return nullptr;
	}
    return nullptr;
}

int BVH::get_num_nodes() const
{
    BVH* l = dynamic_cast<BVH*>(left);
    BVH* r = dynamic_cast<BVH*>(right);
    int num_nodes = 1;
    if(l) num_nodes += l->get_num_nodes();
    if(r) num_nodes += r->get_num_nodes();
    return num_nodes;
}

int BVH::get_num_intersectables() const
{
	BVH* l = dynamic_cast<BVH*>(left);
	BVH* r = dynamic_cast<BVH*>(right);
    if(l && r) return l->get_num_intersectables() + r->get_num_intersectables();
	else if (!l && !r && l != r) return 2;
	else if (!l && !r) return 1;
}
