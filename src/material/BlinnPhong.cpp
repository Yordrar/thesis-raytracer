#include "BlinnPhong.h"

BlinnPhong::BlinnPhong(const Vector3& _albedo, float shininess)
	: Material(_albedo),
	  shininess(shininess)
{
	ambient_albedo = Vector3(0);
	specular_albedo = Vector3(255);
}

BlinnPhong::BlinnPhong(float r, float g, float b, float shininess)
	: Material(r, g, b),
	  shininess(shininess)
{
	ambient_albedo = Vector3(0);
	specular_albedo = Vector3(255);
}

BlinnPhong::~BlinnPhong()
{

}

Ray BlinnPhong::scatter(const Ray& ray, float t, const Vector3& normal)
{
	Vector3 intersection_point = ray.get_origin() + ray.get_direction()*t;
	Vector3 new_ray_direction = normal + Vector3::random_in_unit_sphere();
	return Ray(intersection_point, new_ray_direction);
}

Vector3 BlinnPhong::get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const
{
	Vector3 diffuse_color = Material::get_color(uv, normal, light_vectors, view_vector);

	Vector3 color = ambient_albedo * 0.5f;
	for(auto l : light_vectors) {
		Vector3 half_vector = (l + view_vector).unit();
		color += Math::Fast_Max(l.dot(normal), 0) * diffuse_color  +  0.001f * powf(normal.dot(half_vector), shininess) * specular_albedo;
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
