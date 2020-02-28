#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

class Material
{
public:
	Material();
	virtual ~Material() = 0;

	virtual Ray scatter(Ray ray) = 0;

protected:
	Vector3 albedo;
};

