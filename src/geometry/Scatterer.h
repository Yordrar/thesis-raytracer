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
			if(material->get_texture_map()) new_material->set_texture_map(material->get_texture_map()->get_copy());
			if(material->get_normal_map()) new_material->set_normal_map(material->get_normal_map()->get_copy());
			delete material;
		}
		material = new_material;
	}
};
