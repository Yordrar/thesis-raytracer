#include "Metal.h"

Metal::~Metal()
{

}

Ray Metal::scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity)
{
	float metal_prob = Math::Randf();
	if(metal_prob <= metallicity) {
		Vector3 ray_direction = ray.get_direction();
		Vector3 new_direction = ray_direction - 2*ray_direction.dot(normal)*normal + Vector3::random_in_unit_sphere()*roughness;
		return Ray(ray.get_point(t), new_direction);
	}
	else {
		Vector3 intersection_point = ray.get_origin() + ray.get_direction()*t;
		Vector3 new_ray_direction = normal + Vector3::random_in_unit_sphere();
		return Ray(intersection_point, new_ray_direction);
	}
}
