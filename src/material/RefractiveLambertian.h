#pragma once

#include "Material.h"

#include <math/Vector3.h>

class RefractiveLambertian : public Material
{
public:
	RefractiveLambertian(const Vector3& albedo = Vector3(128), float opacity = 1);
	RefractiveLambertian(float r, float g, float b, float opacity = 1);
	~RefractiveLambertian() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal) override;

	inline bool is_affected_by_shadow_rays() const override {return false;}

	inline float get_opacity() const {return opacity;}
	inline void set_opacity(float value) {opacity = Math::Clampf(value, 0, 1);}

private:
	float opacity = 1;
};

