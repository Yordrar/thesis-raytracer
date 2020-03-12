#pragma once

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Scatterer.h>
#include <geometry/BVH/AxisAlignedBoundingBox.h>

class Sphere : public Entity, public Intersectable, public Scatterer
{
public:
	Sphere(Vector3 _position = Vector3(0, 0, 0), float radius = 0);
	~Sphere() override;

	inline void set_radius(float r) {radius = r;}
	inline float get_radius() const {return radius;}

	// Intersectable interface
	Hit get_intersection(Ray ray) const override;

	// Scatterer interface
	Vector3 get_normal(Vector3 point) const override;

private:
	float radius;
};

