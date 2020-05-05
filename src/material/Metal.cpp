#include "Metal.h"

Metal::~Metal()
{

}

Ray Metal::scatter(const Ray& ray, float t, const Vector3& normal)
{
	Vector3 ray_direction = ray.get_direction();
	Vector3 new_direction = ray_direction - 2*ray_direction.dot(normal)*normal + Vector3::random_in_unit_sphere()*roughness;
	return Ray(ray.get_point(t), new_direction);
}
