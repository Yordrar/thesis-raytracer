#include "Mesh.h"

#include <geometry/mesh/Triangle.h>

#include <chrono>
#include <iostream>

#include <material/Lambertian.h>

#include <geometry/BVH/RandomAxis.h>
#include <geometry/BVH/SurfaceAreaHeuristic.h>

Mesh::Mesh(std::vector<Triangle> triangles)
{
	this->triangles = triangles;
	std::vector<Intersectable*> tris;
	for(Triangle& t : this->triangles) {
		tris.push_back(&t);
		position += ((t.get_vertex0() + t.get_vertex1() + t.get_vertex2()) / 3) / this->triangles.size();
	}
	std::cout << "Building mesh BVH with " << this->triangles.size() << " triangles..." << std::endl;
	std::chrono::steady_clock::time_point bvh_begin = std::chrono::steady_clock::now();
	BVHBuildStrategy* strategy = new SurfaceAreaHeuristic();
	tri_hierarchy = new BVH(tris, strategy);
	delete dynamic_cast<SurfaceAreaHeuristic*>(strategy);
	std::chrono::steady_clock::time_point bvh_end = std::chrono::steady_clock::now();
	std::cout << "BVH built with: " << tri_hierarchy->get_num_nodes()
			  << " internal nodes and " << tri_hierarchy->get_num_intersectables()
			  << " leaf nodes" <<
			  " in " << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_begin).count() << " ms " << std::endl<<std::endl;

	material = new Lambertian;
}

Mesh::~Mesh()
{
	delete tri_hierarchy;
}

Hit Mesh::get_intersection(const Ray& ray) const
{
#if 0
	Hit h(false, nullptr, Vector3(), FLT_MAX);
	for(Triangle t : triangles) {
		Hit hit = t.get_intersection(ray);
		if(hit.is_hit() && hit.get_t() <= h.get_t()) {
			h = hit;
		}
	}
	if(h.is_hit()) {
		h.set_material(material);
		h.set_entity_position(position);
	}
	return h;
#else
	Hit hit = tri_hierarchy->get_intersection(ray);
	if(hit.is_hit()) {
		hit.set_material(material);
		hit.set_entity_position(position);
	}
	return hit;
#endif
}

AxisAlignedBoundingBox Mesh::get_bounding_box() const
{
	return tri_hierarchy->get_bounding_box();
}

void Mesh::translate_global(float delta_x, float delta_y, float delta_z)
{
	Entity::translate_global(delta_x, delta_y, delta_z);

	Vector3 delta(delta_x, delta_y, delta_z);

	std::vector<Intersectable*> tris;
	for(Triangle& t : triangles) {
		t.translate(delta_x, delta_y, delta_z);
		tris.push_back(&t);
	}
	delete tri_hierarchy;
	BVHBuildStrategy* strategy = new RandomAxis();
	tri_hierarchy = new BVH(tris, strategy);
	delete dynamic_cast<RandomAxis*>(strategy);
}

void Mesh::rotate_global(float euler_x, float euler_y, float euler_z)
{
	Entity::rotate_global(euler_x, euler_y, euler_z);

	Quaternion qx = Quaternion::create_rotation(euler_x, Vector3(1, 0, 0));
	Quaternion qy = Quaternion::create_rotation(euler_y, Vector3(0, 1, 0));
	Quaternion qz = Quaternion::create_rotation(euler_z, Vector3(0, 0, 1));
	Quaternion q = qz*(qy*qx).unit();

	std::vector<Intersectable*> tris;
	for(Triangle& t : triangles) {
		t.translate(-position.get_x(), -position.get_y(), -position.get_z());
		t.rotate_vertices(q);
		t.translate(position.get_x(), position.get_y(), position.get_z());
		tris.push_back(&t);
	}
	delete tri_hierarchy;
	BVHBuildStrategy* strategy = new RandomAxis();
	tri_hierarchy = new BVH(tris, strategy);
	delete dynamic_cast<RandomAxis*>(strategy);
}
