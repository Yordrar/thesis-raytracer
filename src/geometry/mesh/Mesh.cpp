#include "Mesh.h"

#include <geometry/mesh/Triangle.h>

#include <chrono>
#include <iostream>

Mesh::Mesh(std::vector<Triangle> triangles)
{
	this->triangles = triangles;
	std::vector<Intersectable*> tris;
	for(Triangle& t : this->triangles) {
		tris.push_back(&t);
	}
	tri_hierarchy = new BVH(tris);
}

Mesh::~Mesh()
{
	delete tri_hierarchy;
}

Hit Mesh::get_intersection(const Ray& ray) const
{
	Hit hit = tri_hierarchy->get_intersection(ray);
	if(hit.is_hit()) {
		hit.set_material(material);
		if(texture_map) {
			hit.set_texture(texture_map);
		}
	}
	return hit;
}

AxisAlignedBoundingBox Mesh::get_bounding_box() const
{
	return tri_hierarchy->get_bounding_box();
}
