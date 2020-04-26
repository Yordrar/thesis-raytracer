#pragma once

#include <vector>

#include <math/Vector3.h>
#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Scatterer.h>
#include <geometry/mesh/Triangle.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>
#include <geometry/BVH/BVH.h>

class Mesh : public Entity, public Intersectable, public Scatterer
{
public:
	Mesh(std::vector<Triangle> triangles);
	~Mesh() override;

	// TODO: override the Entity interface to calculate the position as
	// mean of the vertices' positions

	// Intersectable interface
	Hit get_intersection(const Ray& ray) const override;
	AxisAlignedBoundingBox get_bounding_box() const override;

	void translate_global(float delta_x, float delta_y, float delta_z) override;
	void rotate_global(float euler_x, float euler_y, float euler_z) override;

private:
	// The representation is a simple Indexed Triangle List
	// The vertices are stored without repetition (some triangles may share vertices)
	// In another list, indices to the vertex list are stored. These are what define the triangles
	std::vector<Triangle> triangles;
	BVH* tri_hierarchy;
};
