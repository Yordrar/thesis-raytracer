#pragma once

#include <material/Material.h>

class Emissive : public Material
{
public:
	using Material::Material;
	~Emissive() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity) override;
	Vector3 get_emission_color(const Ray& ray, float t, const Vector3& normal) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}

	inline float get_intensity() const {return intensity;}
	inline void set_intensity(float value) {intensity = value;}

private:
    float intensity = 1;
};

