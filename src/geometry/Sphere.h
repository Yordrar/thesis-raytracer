#pragma once

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>

class Sphere : public Entity, public Intersectable
{
public:
	Sphere(Vector3 _position = Vector3(0, 0, 0), float radius = 0);
	~Sphere() override;

	inline void set_radius(float r) {radius = r;}
	inline float get_radius() const {return radius;}

	float get_intersection(Ray ray) const override;
	Vector3 get_normal(Vector3 point) const override;
	Ray scatter(Ray ray, float t) const override;

private:
	float radius;
};

