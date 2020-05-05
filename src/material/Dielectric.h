#pragma once

#include <material/Material.h>

class Dielectric : public Material
{
public:
	using Material::Material;
	~Dielectric() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}

	inline float get_refraction_index() const {return refraction_index;}
	inline void set_refraction_index(float value) {refraction_index = value;}

private:
	float refraction_index = 1.5f;

	float get_discriminant(const Ray& ray, const Vector3& normal, float ni_over_nt);
	float schlick(float cosine);
};

