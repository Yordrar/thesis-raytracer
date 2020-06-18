#include "BlinnPhong.h"

#include <iostream>

BlinnPhong::~BlinnPhong()
{

}

float BlinnPhong::fresnel(float n1, float n2, Vector3 normal, Vector3 incident)
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

Ray BlinnPhong::scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity)
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

Vector3 BlinnPhong::get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const
{
    Vector3 diffuse_color = Material::get_color(uv, normal, light_vectors, view_vector);
	float roughness = this->roughness;
	if(roughness_map)
		roughness = roughness_map->get_pixel_color_bilinear_interp(uv.get_x() * roughness_map->get_width(),
																   (1.0f-uv.get_y()) * roughness_map->get_height()).get_x();

	Vector3 color = ambient_albedo * 0.5f;
    for(auto l : light_vectors) {
        Vector3 half_vector = (l + view_vector).unit();
        color += (Math::Fast_Max(l.dot(normal), 0) * diffuse_color  +
                 Math::Map(roughness, 0, 1, 0.003f, 0) * powf(normal.dot(half_vector), Math::Map(roughness, 0, 1, 200, 2)) * specular_albedo);
	}
    return color;
}

Vector3 BlinnPhong::get_ambient_albedo() const
{
	return ambient_albedo;
}

void BlinnPhong::set_ambient_albedo(const Vector3& value)
{
	ambient_albedo = value;
}

Vector3 BlinnPhong::get_specular_albedo() const
{
	return specular_albedo;
}

void BlinnPhong::set_specular_albedo(const Vector3& value)
{
	specular_albedo = value;
}

void BlinnPhong::set_texture_map(Image* value)
{
	Material::set_texture_map(value);
	if(texture_map)
		ambient_albedo = texture_map->get_mean();
}
