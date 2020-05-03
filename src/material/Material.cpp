#include "Material.h"

Material::Material(const Vector3& albedo)
{
	this->albedo = albedo/255;
	roughness = 0;
	metallicity = 1;
	reflectance = 1;
}

Material::Material(float r, float g, float b)
{
	albedo = Vector3(r, g, b)/255;
	roughness = 0;
	metallicity = 1;
	reflectance = 1;
}

Vector3 Material::get_emission_color(const Ray& ray, float t, const Vector3& normal)
{
	return Vector3();
}

Material::~Material()
{
	if(texture_map) delete texture_map;
	if(normal_map) delete normal_map;
}

Vector3 Material::get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const
{
	if(texture_map) {
		return texture_map->get_pixel_color(static_cast<int>(uv.get_x() * texture_map->get_width()),
											static_cast<int>((1.0f-uv.get_y()) * texture_map->get_height()));
	}
	else {
		return albedo;
	}
}

Vector3 Material::get_normal(const Vector3& uv) const
{
	if(normal_map) {
		return normal_map->get_pixel_color(static_cast<int>(uv.get_x() * normal_map->get_width()),
										   static_cast<int>((1.0f-uv.get_y()) * normal_map->get_height()));
	}
	else {
		return Vector3(0);
	}
}
