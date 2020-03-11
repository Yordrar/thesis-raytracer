#pragma once

#include <material/Material.h>

#include <geometry/BVH/AxisAlignedBoundingBox.h>

#include <utility>

class Intersectable
{
protected:
	Intersectable();
	Intersectable(Material* m);
	virtual ~Intersectable() = 0;

	AxisAlignedBoundingBox bounding_box;
	Material* material;

public:
	virtual std::pair<const Intersectable*, float> get_intersection(Ray ray) const = 0;
	virtual Vector3 get_normal(Vector3 point) const = 0;
	virtual AxisAlignedBoundingBox get_bounding_box() const {return bounding_box;}

	inline Material* get_material() const {return material;}
	inline void set_material(Material* m) {
		if(material) delete material;
		material = m;
	}
};

