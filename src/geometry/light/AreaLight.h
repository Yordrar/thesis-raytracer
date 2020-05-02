#pragma once

#include <math/Vector3.h>

#include <geometry/Entity.h>
#include <geometry/Emitter.h>

class AreaLight : public Entity, public Emitter
{
public:
	AreaLight(Vector3 albedo = Vector3(255, 255, 255), float intensity = 1);
	~AreaLight() override;

	// Entity interface
	void rotate_global(float euler_x, float euler_y, float euler_z) override;

	// Emitter interface
	Vector3 get_emission_color(Vector3 position) const override;
	Ray get_shadow_ray(Vector3 position) const override;
	float get_distance(Vector3 position) const override;

	inline float get_size() const {return size;}
	inline void set_size(float value) {size = value;}

private:
	float intensity;
	float size = 1;
};

