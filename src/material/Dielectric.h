#pragma once

#include <material/Material.h>

class Dielectric : public Material
{
public:
	Dielectric(Vector3 albedo = Vector3(128, 128, 128), float refraction_index = 1.0f);
	Dielectric(float r, float g, float b, float refraction_index = 1.0f);
	~Dielectric() override;

	Ray scatter(Ray ray, float t, Vector3 normal) override;

private:
	float refraction_index;
};

