#include "Lambertian.h"

Lambertian::~Lambertian()
{

}

float Lambertian::fresnel(float n1, float n2, Vector3 normal, Vector3 incident)
{
    // Schlick aproximation
    float r0 = (n1-n2) / (n1+n2);
    r0 *= r0;
    float cosX = -normal.dot(incident);
    if (n1 > n2)
    {
        float n = n1/n2;
        float sinT2 = n*n*(1.0-cosX*cosX);
        // Total internal reflection
        if (sinT2 > 1.0)
            return 1.0;
        cosX = sqrt(1.0-sinT2);
    }
    float x = 1.0-cosX;
    float ret = r0+(1.0-r0)*x*x*x*x*x;

    // adjust reflect multiplier for object reflectivity
	ret = (reflectance + (1.0-reflectance) * ret);
    return ret;
}

Ray Lambertian::scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity)
{
    float reflectance_prob = Math::Randf();
    if(reflectance_prob <= fresnel(1, 1.125f, normal, ray.get_direction())) {
		Vector3 ray_direction = ray.get_direction();
		Vector3 new_direction = ray_direction - 2*ray_direction.dot(normal)*normal;
		return Ray(ray.get_point(t), new_direction);
	}
	else {
        Vector3 intersection_point = ray.get_point(t);
		Vector3 new_ray_direction = normal + Vector3::random_in_unit_sphere();
		return Ray(intersection_point, new_ray_direction);
    }
}
