#pragma once

#include "Material.h"

#include <math/Vector3.h>

class RefractiveLambertian : public Material
{
public:
	using Material::Material;
	~RefractiveLambertian() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal) override;
};

