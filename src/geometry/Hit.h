#pragma once

#include <geometry/Intersectable.h>

#include <material/Material.h>

class Hit
{
public:
	Hit();
	Hit(Intersectable* i, float t, Material* m);

	inline Intersectable* get_intersectable() const {return intersectable;}
	inline float get_t() const {return t;}
	inline Material* get_material() const {return material;}

private:
	Intersectable* intersectable;
	float t;
	Material* material;
};

