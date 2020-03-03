#pragma once

#include <material/Material.h>

class Intersectable
{
protected:
	Intersectable();
	Intersectable(Material* m);
	virtual ~Intersectable() = 0;

	Material* material;

public:
	virtual float get_intersection(Ray ray) const = 0;
	virtual Vector3 get_normal(Vector3 point) const = 0;

	inline Material* get_material() const {return material;}
	inline void set_material(Material* m) {
		delete material;
		material = m;
	}
	virtual Ray scatter(Ray ray, float t) const = 0;
};

