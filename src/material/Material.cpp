#include "Material.h"

Material::Material(const Vector3& albedo)
{
	this->albedo = albedo/255;
	roughness = 0;
	metallicity = 1;
    reflectance = 0;
}

Material::Material(float r, float g, float b)
{
	albedo = Vector3(r, g, b)/255;
	roughness = 0;
	metallicity = 1;
    reflectance = 0;
}

Vector3 Material::get_emission_color(const Ray& ray, float t, const Vector3& normal)
{
	return Vector3();
}

Material::~Material()
{
	if(texture_map) delete texture_map;
	if(normal_map) delete normal_map;
	if(roughness_map) delete roughness_map;
	if(metallicity_map) delete metallicity_map;
	if(ao_map) delete ao_map;
}

Vector3 Material::get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const
{
	if(texture_map) {
		Vector3 texture_color = texture_map->get_pixel_color_bilinear_interp(uv.get_x() * texture_map->get_width(),
																			 (1.0f-uv.get_y()) * texture_map->get_height());
		if(ao_map) {
			texture_color *= ao_map->get_pixel_color_bilinear_interp(uv.get_x() * ao_map->get_width(),
																	 (1.0f-uv.get_y()) * ao_map->get_height());
		}

		return texture_color;
	}
	else {
		return albedo;
	}
}

Vector3 Material::get_normal(const Vector3& uv) const
{
	if(normal_map) {
		return normal_map->get_pixel_color_bilinear_interp(uv.get_x() * normal_map->get_width(),
														   (1.0f-uv.get_y()) * normal_map->get_height());
	}
	else {
		return Vector3(0);
	}
}

float Material::get_roughness_from_map(const Vector3& uv) const
{
	if(roughness_map) {
		return roughness_map->get_pixel_color_bilinear_interp(uv.get_x() * roughness_map->get_width(),
															  (1.0f-uv.get_y()) * roughness_map->get_height()).get_x();
	}
	else {
		return roughness;
	}
}

float Material::get_metallicity_from_map(const Vector3& uv) const
{
	if(metallicity_map) {
		return metallicity_map->get_pixel_color_bilinear_interp(uv.get_x() * metallicity_map->get_width(),
																(1.0f-uv.get_y()) * metallicity_map->get_height()).get_x();
	}
	else {
		return metallicity;
	}
}
