#pragma once

#include <math/Vector3.h>

#include <geometry/Emitter.h>

class DirectionalLight : public Emitter
{
public:
	DirectionalLight(Vector3 albedo = Vector3(255, 255, 255), Vector3 direction = Vector3(-1));
	~DirectionalLight() override;

	// Emitter interface
	Vector3 get_emission_color(Vector3 position) const override;
	Ray get_shadow_ray(Vector3 position) const override;
	float get_distance(Vector3 position) const override;

private:
	Vector3 direction;
};

