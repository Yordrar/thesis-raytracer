#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>
#include <geometry/Entity.h>

#include <list>

class Camera
{
public:
	Camera(Vector3 position = Vector3(), Vector3 direction = Vector3(0, 0, -1), int width = 0, int height = 0);

	inline Vector3 get_position() const {return position;}
	inline Vector3 get_direction() const {return direction;}
	inline int get_width() const {return width;}
	inline int get_height() const {return height;}

	Vector3 get_color(float x, float y, const std::list<Entity*> entities) const;

private:
	Vector3 position, direction;
	int width, height;

	Vector3 shoot_ray(Ray r, const std::list<Entity*> entities) const ;
};

