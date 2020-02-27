#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 direction, int width, int height)
	: position(position),
	  direction(direction),
	  width(width),
	  height(height)
{

}

Ray Camera::get_ray(float x, float y) const
{
	float fov = 90.0f;
	float fovx = 2*atanf(width/(2*fov));
	float fovy = 2*atanf(height/(2*fov));

	float plane_width = 2;
	float plane_height = 2;
	if(fovx > fovy) {
		plane_height = height*(plane_width/width);
	}
	else if(fovx < fovy) {
		plane_width = width*(plane_height/height);
	}
	Vector3 upper_left_corner = Vector3(0-plane_width/2, 0+plane_height/2, -1);
	float u = x / float(width);
	float v = y / float(height);
	return Ray(position, upper_left_corner + Vector3(u*plane_width, -v*plane_height, 0));
}
