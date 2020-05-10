#pragma once

#include <material/Material.h>

#include <image/Image.h>

class Scatterer
{
protected:
	Scatterer(Material* m = nullptr);
	virtual ~Scatterer() = 0;

	Material* material;

public:
	inline Material* get_material() const {return material;}
	inline void set_material(Material* new_material) {
		if(material) {
			new_material->set_albedo(material->get_albedo());
			new_material->set_roughness(material->get_roughness());
			new_material->set_metallicity(material->get_metallicity());
			new_material->set_reflectance(material->get_reflectance());
			if(material->get_texture_map()) new_material->set_texture_map(material->get_texture_map()->get_copy());
			if(material->get_normal_map()) new_material->set_normal_map(material->get_normal_map()->get_copy());
			if(material->get_roughness_map()) new_material->set_roughness_map(material->get_roughness_map()->get_copy());
			if(material->get_metallicity_map()) new_material->set_metallicity_map(material->get_metallicity_map()->get_copy());
			if(material->get_ao_map()) new_material->set_ao_map(material->get_ao_map()->get_copy());
			delete material;
		}
		material = new_material;
	}
};
