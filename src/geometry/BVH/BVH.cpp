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

	if (box_left.get_min_corner().get_x() < box_right.get_min_corner().get_x())
		return -1;
	else
		return 1;
}

int box_y_compare(const void* a, const void* b) {
	Intersectable* ah = *(Intersectable**)a;
	Intersectable* bh = *(Intersectable**)b;

	AxisAlignedBoundingBox box_left = ah->get_bounding_box();
	AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if (box_left.get_min_corner().get_y() < box_right.get_min_corner().get_y())
		return -1;
	else
		return 1;
}

int box_z_compare(const void* a, const void* b) {
	Intersectable* ah = *(Intersectable**)a;
	Intersectable* bh = *(Intersectable**)b;

	AxisAlignedBoundingBox box_left = ah->get_bounding_box();
	AxisAlignedBoundingBox box_right = bh->get_bounding_box();

	if (box_left.get_min_corner().get_z() < box_right.get_min_corner().get_z())
		return -1;
	else
		return 1;
}

BVH::BVH(std::vector<Intersectable*> intersectables)
{
	int axis = static_cast<int>(Math::Randf()*3.0f);
	int num_elem = static_cast<int>(intersectables.size());

	switch(axis) {
	case 0:
		std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), box_x_compare);
		break;
	case 1:
		std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), box_y_compare);
		break;
	case 2:
		std::qsort(intersectables.data(), num_elem, sizeof(Intersectable*), box_z_compare);
		break;
	}

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
		int half_size = num_elem / 2;
		auto half = intersectables.begin() + half_size;

		std::vector<Intersectable*> left_half(intersectables.begin(), half);
		left = new BVH(left_half);

		std::vector<Intersectable*> right_half(half, intersectables.end());
		right = new BVH(right_half);
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

Hit BVH::get_intersection(Ray ray) const
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
		if(hit_right.is_hit()) {
			if(hit_right.get_t() < hit_left.get_t()) {
				return hit_right;
			}
		}
		return hit_left;
	}
	return Hit();
}

int BVH::count() const
{
	if(left == right) return 1;
	BVH* l = dynamic_cast<BVH*>(left);
	BVH* r = dynamic_cast<BVH*>(right);
	if(l && r) return l->count() + r->count();
	else if (!l && !r) return 2;
}
