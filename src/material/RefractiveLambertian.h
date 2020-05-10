#pragma once

#include "Material.h"

#include <math/Vector3.h>

class RefractiveLambertian : public Material
{
public:
	using Material::Material;
	~RefractiveLambertian() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}
};

