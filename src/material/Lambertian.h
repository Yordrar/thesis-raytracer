#pragma once

#include <material/Material.h>

class Lambertian : public Material
{
public:
	using Material::Material;
	~Lambertian() override;

	Ray scatter(Ray ray, float t, Vector3 normal) override;
};

