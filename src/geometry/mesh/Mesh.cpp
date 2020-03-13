#include "Mesh.h"

#include <geometry/mesh/Triangle.h>

#include <chrono>
#include <iostream>

Mesh::Mesh(std::vector<Triangle>& triangles)
{
	this->triangles = triangles;
	std::vector<Intersectable*> tris;
	for(Triangle& t : this->triangles) {
		tris.push_back(&t);
	}
	tri_hierarchy = new BVH(tris, static_cast<int>(tris.size()));
}

Mesh::~Mesh()
{

}

Hit Mesh::get_intersection(Ray ray) const
{
	Hit hit = tri_hierarchy->get_intersection(ray);
	if(hit.is_hit()) {
		hit.set_material(material);
	}
	return hit;
}

AxisAlignedBoundingBox Mesh::get_bounding_box() const
{
	return tri_hierarchy->get_bounding_box();
}

Vector3 Mesh::get_normal(Vector3 point) const
{
	return Vector3(); // Meshes don't have normals
}
