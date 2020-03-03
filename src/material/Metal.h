#pragma once

#include <material/Material.h>

class Metal : public Material
{
public:
	Metal(Vector3 albedo = Vector3(128, 128, 128), float roughness = 0.0f);
	Metal(float r, float g, float b, float roughness = 0.0f);
	~Metal() override;

	Ray scatter(Ray ray, float t, Vector3 normal) override;

private:
	float roughness;
};

