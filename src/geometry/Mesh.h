#pragma once

#include <vector>

#include <math/Vector3.h>
#include <geometry/Entity.h>
#include <geometry/Intersectable.h>

class Mesh : public Entity, public Intersectable
{
public:
	Mesh();

	// TODO: override the Entity interface to calculate the position as
	// mean of the vertices' positions

	// Intersectable interface
	float get_intersection(Ray ray) const override;
	Ray scatter(Ray ray, float t) const override;

	void add_vertex(Vector3 vertex);
	void add_triangle(int v0, int v1, int v2);

private:
	// The representation is a simple Indexed Triangle List
	// The vertices are stored without repetition (some triangles may share vertices)
	// In another list, indices to the vertex list are stored. These are what define the triangles
	std::vector<Vector3> vertices;
	std::vector<int> triangles;
};

