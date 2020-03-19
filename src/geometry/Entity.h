#pragma once

#include <math/Vector3.h>
#include <math/Quaternion.h>
#include <math/Ray.h>

class Entity
{
protected:
	Entity(Vector3 position = Vector3(0, 0, 0), Quaternion orientation = Quaternion(0, 0, 1, 0));
	virtual ~Entity() = 0;

	Vector3 position;
	Quaternion orientation;

public:
	inline void set_position(const Vector3& new_position) {position = new_position;}
	inline Vector3 get_position() const {return position;}

	inline void set_rotation(const Quaternion& new_rotation) {orientation = new_rotation;}
	inline Quaternion get_rotation() const {return orientation;}

	void translate(float delta_x, float delta_y, float delta_z);
	void translate(const Vector3& delta);
	void rotate(float euler_x, float euler_y, float euler_z);
	void rotate(const Quaternion& orientation);
};
