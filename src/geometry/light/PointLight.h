#pragma once

#include <math/Vector3.h>

#include <geometry/Entity.h>
#include <geometry/Emitter.h>

class PointLight : public Entity, public Emitter
{
public:
	PointLight(Vector3 albedo = Vector3(255, 255, 255), float intensity = 1);
	~PointLight() override;

	// Emitter interface
	Vector3 get_emission_color(Vector3 position, Vector3 normal) const override;
	Ray get_shadow_ray(Vector3 position) const override;
	float get_distance(Vector3 position) const override;

private:
	float intensity;
};

