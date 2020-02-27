#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>

class Camera
{
public:
	Camera(Vector3 position = Vector3(), Vector3 direction = Vector3(0, 0, -1), int width = 0, int height = 0);

	inline Vector3 get_position() const {return position;}
	inline Vector3 get_direction() const {return direction;}
	inline int get_width() const {return width;}
	inline int get_height() const {return height;}

	Ray get_ray(float x, float y) const;

private:
	Vector3 position, direction;
	int width, height;
};

