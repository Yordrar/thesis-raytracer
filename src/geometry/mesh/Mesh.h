#pragma once

#include <vector>

#include <math/Vector3.h>
#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class Mesh : public Entity, public Intersectable
{
public:
	Mesh();
	~Mesh() override;

	void add_vertex(Vector3 vertex);
	void add_triangle(int v0, int v1, int v2);

	// TODO: override the Entity interface to calculate the position as
	// mean of the vertices' positions

	// Intersectable interface
	std::pair<const Intersectable*, float> get_intersection(Ray ray) const override;
	AxisAlignedBoundingBox get_bounding_box() const override;
	Vector3 get_normal(Vector3 point) const override;

private:
	// The representation is a simple Indexed Triangle List
	// The vertices are stored without repetition (some triangles may share vertices)
	// In another list, indices to the vertex list are stored. These are what define the triangles
	std::vector<Vector3> vertices;
	std::vector<int> triangles;
};
