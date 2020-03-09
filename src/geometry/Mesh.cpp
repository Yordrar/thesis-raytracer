#include "Mesh.h"

#include <geometry/Triangle.h>

Mesh::Mesh()
{

}

float Mesh::get_intersection(Ray ray) const
{
	for(int i = 0; i < static_cast<int>(triangles.size()/3); i++) {
		Vector3 v0 = vertices[triangles[i*3]];
		Vector3 v1 = vertices[triangles[i*3 + 1]];
		Vector3 v2 = vertices[triangles[i*3 + 2]];
		Triangle tri(v0, v1, v2);
		// Discard the triangle if it is not facing the camera
		if(tri.get_normal().dot(ray.get_direction()) > 0.0f) continue;
		float t = tri.get_intersection(ray);
		if(t != 0.0f) return t;
	}
	return 0.0f;
}

Ray Mesh::scatter(Ray ray, float t) const
{
	for(int i = 0; i < static_cast<int>(triangles.size()/3); i++) {
		Vector3 v0 = vertices[triangles[i*3]];
		Vector3 v1 = vertices[triangles[i*3 + 1]];
		Vector3 v2 = vertices[triangles[i*3 + 2]];
		Triangle tri(v0, v1, v2);
		// Discard the triangle if it is not facing the camera
		if(tri.get_normal().dot(ray.get_direction()) > 0.0f) continue;
		float tri_t = tri.get_intersection(ray);
		if(Math::Float_Eq(t, tri_t)) {
			return material->scatter(ray, t, tri.get_normal());
		}
	}
	return ray;
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
