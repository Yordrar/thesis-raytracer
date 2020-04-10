#pragma once

#include <material/Material.h>

class Emissive : public Material
{
public:
	Emissive(const Vector3& albedo = Vector3(128, 128, 128), float intensity = 1.0f);
	Emissive(float r, float g, float b, float intensity = 1.0f);
	~Emissive() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal) override;
	Vector3 get_emission_color(const Ray& ray, float t, const Vector3& normal) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}

private:
	float intensity;
};

