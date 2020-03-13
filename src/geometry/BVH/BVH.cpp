#include "BVH.h"

#include <algorithm>
#include <iostream>

BVH::BVH()
{

}

int box_x_compare(const void* a, const void* b) {
	Intersectable* ah = *(Intersectable**)a;
	Intersectable* bh = *(Intersectable**)b;

	AxisAlignedBoundingBox box_left = ah->get_bounding_box();
	AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if ((box_left.get_min_corner() == Vector3() && box_left.get_max_corner() == Vector3()) ||
		(box_right.get_min_corner() == Vector3() && box_right.get_max_corner() == Vector3()))
		std::cerr << "no bounding box in bvh_node constructor\n";

	if (box_left.get_min_corner().get_x() - box_right.get_max_corner().get_x() < 0.0f)
		return -1;
	else
		return 1;
}

int box_y_compare(const void* a, const void* b) {
	Intersectable* ah = *(Intersectable**)a;
	Intersectable* bh = *(Intersectable**)b;

	AxisAlignedBoundingBox box_left = ah->get_bounding_box();
	AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if ((box_left.get_min_corner() == Vector3() && box_left.get_max_corner() == Vector3()) ||
		(box_right.get_min_corner() == Vector3() && box_right.get_max_corner() == Vector3()))
		std::cerr << "no bounding box in bvh_node constructor\n";

	if (box_left.get_min_corner().get_y() - box_right.get_max_corner().get_y() < 0.0f)
		return -1;
	else
		return 1;
}

int box_z_compare(const void* a, const void* b) {
	Intersectable* ah = *(Intersectable**)a;
	Intersectable* bh = *(Intersectable**)b;

	AxisAlignedBoundingBox box_left = ah->get_bounding_box();
	AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if ((box_left.get_min_corner() == Vector3() && box_left.get_max_corner() == Vector3()) ||
		(box_right.get_min_corner() == Vector3() && box_right.get_max_corner() == Vector3()))
		std::cerr << "no bounding box in bvh_node constructor\n";

	if (box_left.get_min_corner().get_z() - box_right.get_max_corner().get_z() < 0.0f)
		return -1;
	else
		return 1;
}

BVH::BVH(std::vector<Intersectable*> intersectables, int num_elem)
{
	int axis = static_cast<int>(Math::Randf()*3.0f);

	switch(axis) {
	case 0:
		std::qsort(intersectables.data(), intersectables.size(), sizeof(Intersectable*), box_x_compare);
		break;
	case 1:
		std::qsort(intersectables.data(), intersectables.size(), sizeof(Intersectable*), box_y_compare);
		break;
	case 2:
		std::qsort(intersectables.data(), intersectables.size(), sizeof(Intersectable*), box_z_compare);
		break;
	}

	if (num_elem == 1) {
		left = intersectables[0];
		right = intersectables[0];
	}
	else if (num_elem == 2) {
		left = intersectables[0];
		right = intersectables[1];
	}
	else {
		size_t half_size = intersectables.size() / 2;
		auto half = intersectables.begin() + half_size;

		std::vector<Intersectable*> left_half(intersectables.begin(), half);
		left = new BVH(left_half, static_cast<int>(left_half.size()));

		std::vector<Intersectable*> right_half(half, intersectables.end());
		right = new BVH(right_half, static_cast<int>(right_half.size()));
	}

	AxisAlignedBoundingBox box_left = left->get_bounding_box();
	AxisAlignedBoundingBox box_right = right->get_bounding_box();

	if ((box_left.get_min_corner() == Vector3() && box_left.get_max_corner() == Vector3()) ||
		(box_right.get_min_corner() == Vector3() && box_right.get_max_corner() == Vector3())) {
		std::cerr << "no bounding box in bvh_node constructor\n";
	}

	bounding_box = AxisAlignedBoundingBox::surrounding_box(box_left, box_right);
}

BVH::~BVH()
{
	BVH* l = dynamic_cast<BVH*>(left);
	BVH* r = dynamic_cast<BVH*>(right);

	if(l) delete l;
	if(r) delete r;
}

Hit BVH::get_intersection(Ray ray) const
{
	if(bounding_box.hit(ray, 0.001f, FLT_MAX)) {
		if(left == right) {
			return left->get_intersection(ray);
		}
		Hit hit_left = left->get_intersection(ray);
		Hit hit_right = right->get_intersection(ray);
		if(hit_left.is_hit() && hit_right.is_hit()) {
			if(hit_left.get_t() <= hit_right.get_t()) {
				return hit_left;
			}
			return hit_right;
		}
		else if (hit_left.is_hit()) {
			return hit_left;
		}
		else if (hit_right.is_hit()) {
			return hit_right;
		}
	}
	return Hit();
}
