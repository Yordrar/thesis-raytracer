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
	inline void set_material(Material* m) {
		if(material) {
			if(material->get_texture_map()) m->set_texture_map(m->get_texture_map()->get_copy());
			if(material->get_normal_map()) m->set_normal_map(m->get_normal_map()->get_copy());
			delete material;
		}
		material = m;
	}
};
