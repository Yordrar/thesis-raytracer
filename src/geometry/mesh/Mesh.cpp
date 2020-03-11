#include "Mesh.h"

#include <geometry/mesh/Triangle.h>

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

std::pair<const Intersectable*, float> Mesh::get_intersection(Ray ray) const
{
	for(int i = 0; i < static_cast<int>(triangles.size()/3); i++) {
		Vector3 v0 = vertices[triangles[i*3]];
		Vector3 v1 = vertices[triangles[i*3 + 1]];
		Vector3 v2 = vertices[triangles[i*3 + 2]];
		Triangle* tri = new Triangle(v0, v1, v2);
		tri->set_material(material);
		// Discard the triangle if it is not facing the camera
		if(tri->get_normal(Vector3()).dot(ray.get_direction()) > 0.0f) continue;
		std::pair<const Intersectable*, float> intersects = tri->get_intersection(ray);
		if(intersects.first) {
			return intersects;
		}
	}
	return std::make_pair(nullptr, 0.0f);
}

AxisAlignedBoundingBox Mesh::get_bounding_box() const
{
	Vector3 min_corner(FLT_MAX);
	Vector3 max_corner(FLT_MIN);
	for(Vector3 vertex : vertices) {
		min_corner = min_corner.min(vertex);
		max_corner = max_corner.max(vertex);
	}
	return AxisAlignedBoundingBox(min_corner, max_corner);
}

void Mesh::add_vertex(Vector3 vertex)
{
	vertices.push_back(vertex);
}

void Mesh::add_triangle(int v0, int v1, int v2)
{
	triangles.push_back(v0);
	triangles.push_back(v1);
	triangles.push_back(v2);
}


Vector3 Mesh::get_normal(Vector3 point) const
{
	return Vector3(); // Meshes don't have normals
}
