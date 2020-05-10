#pragma once

#include <material/Material.h>

class Metal : public Material
{
public:
	using Material::Material;
	~Metal() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}
};

