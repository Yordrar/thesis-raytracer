#pragma once

#include <geometry/Intersectable.h>

class Emitter
{
public:
	Emitter(Vector3 albedo = Vector3(255, 255, 255));
	virtual ~Emitter() = 0;

	virtual Vector3 get_emission_color(Vector3 position) const = 0;
	virtual Ray get_shadow_ray(Vector3 position) const = 0;
	virtual float get_distance(Vector3 position) const = 0;

protected:
	Vector3 albedo;
};

