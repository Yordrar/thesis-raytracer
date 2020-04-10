#pragma once

#include <material/Material.h>

class Dielectric : public Material
{
public:
	Dielectric(const Vector3& albedo = Vector3(128, 128, 128), float refraction_index = 1.0f);
	Dielectric(float r, float g, float b, float refraction_index = 1.0f);
	~Dielectric() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}

private:
	float refraction_index;

	float get_discriminant(const Ray& ray, const Vector3& normal, float ni_over_nt);
	float schlick(float cosine);
};

