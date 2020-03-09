#pragma once

#include <geometry/Intersectable.h>
#include <math/Vector3.h>

class Triangle
{
public:
	Triangle(Vector3 v0, Vector3 v1, Vector3 v2);

	float get_intersection(Ray ray) const;
	Vector3 get_normal() const;

private:
	Vector3 v0, v1, v2;
	Vector3 edge01, edge02;
	Vector3 normal;
};

