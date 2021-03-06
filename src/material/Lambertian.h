#pragma once

#include <material/Material.h>

class Lambertian : public Material
{
public:
	using Material::Material;
	~Lambertian() override;

    Ray scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity) override;
	inline bool is_affected_by_shadow_rays() const override {return true;}

private:
    float fresnel(float n1, float n2, Vector3 normal, Vector3 incident);
};

