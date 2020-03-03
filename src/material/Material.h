#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

class Material
{
public:
	Material();
	Material(Vector3 albedo);
	Material(float r, float g, float b);
	virtual ~Material() = 0;

	virtual Ray scatter(Ray ray) = 0;

protected:
	Vector3 albedo;
};

