#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

class Entity
{
protected:
	Entity(Vector3 position = Vector3(0, 0, 0));
	virtual ~Entity() = 0;

	Vector3 position;

public:
	inline void set_position(Vector3 new_position) {position = new_position;}
	inline Vector3 get_position() const {return position;}

	virtual float get_intersection(Ray ray) const = 0;
	virtual Vector3 get_normal(Vector3 point) const = 0;
};

