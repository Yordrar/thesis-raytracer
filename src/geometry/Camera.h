#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>
#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Emitter.h>
#include <geometry/BVH/BVH.h>
#include <geometry/Sphere.h>

#include <vector>

class Camera : public Entity
{
public:
	Camera(int width = 0, int height = 0, float vertical_fov = 45.0f);

	inline int get_width() const {return width;}
	void set_width(int new_width);
	inline int get_height() const {return height;}
	void set_height(int new_height);
	void set_width_and_height(int new_width, int new_height);

	inline Vector3 get_up() const {return up;}
	inline Vector3 get_right() const {return right;}

	Vector3 get_color(float x, float y, const BVH& intersectables, const std::vector<Emitter*>& emitters) const;
	Vector3 get_color_preview(float x, float y, const BVH& intersectables) const;

	void translate_global(float delta_x, float delta_y, float delta_z);
	void translate_global(const Vector3& delta);
	void rotate(float euler_x, float euler_y, float euler_z);
	void rotate(const Quaternion& rotation);

private:
	int width, height;
	float vfov, plane_width, plane_height, half_width, half_height;
	Vector3 upper_left_corner;
	Vector3 up, right;

	Vector3 get_color_recursive(const Ray& r, const BVH& intersectables, const std::vector<Emitter*>& emitters, int depth) const;
	Vector3 get_shadow_ray_color(Vector3 origin, Vector3 normal, const BVH& intersectables, const std::vector<Emitter*>& emitters) const;
	void recalculate_parameters();
	inline Ray get_ray(float u, float v) const {return Ray(position, upper_left_corner + u*right*plane_width + -v*up*plane_height);}
};

