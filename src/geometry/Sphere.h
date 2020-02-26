#pragma once

#include <geometry/Entity.h>

class Sphere : public Entity
{
public:
	Sphere(Vector3 _position = Vector3(0, 0, 0), float radius = 0);
	~Sphere() override;

	inline void set_radius(float r) {radius = r;}
	inline float get_radius() const {return radius;}

	float intersects(Ray ray) const override;
	Vector3 get_normal(Vector3 point) const override;

private:
	float radius;
};

