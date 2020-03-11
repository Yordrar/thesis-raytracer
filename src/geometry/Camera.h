#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>
#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/BVH/BVH.h>

#include <vector>

class Camera
{
public:
	Camera(Vector3 position = Vector3(), Vector3 direction = Vector3(0, 0, -1), int width = 0, int height = 0, float vertical_fov = 45.0f);

	inline Vector3 get_position() const {return position;}
	inline Vector3 get_direction() const {return direction;}
	inline int get_width() const {return width;}
	inline int get_height() const {return height;}

	Vector3 get_color(float x, float y, const BVH intersectables) const;

private:
	Vector3 position, direction;
	int width, height;
	float vfov, plane_width, plane_height;
	Vector3 upper_left_corner;


	Vector3 shoot_ray(Ray r, const BVH intersectables, int depth) const ;
};

