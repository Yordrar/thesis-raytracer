#pragma once

#include <material/Material.h>

#include <image/Image.h>

class Scatterer
{
protected:
	Scatterer(Material* m = nullptr, Image* i = nullptr);
	virtual ~Scatterer() = 0;

	Material* material;
	Image* texture_map;

public:
	inline Material* get_material() const {return material;}
	inline void set_material(Material* m) {
		if(material) delete material;
		material = m;
	}
	inline Image* get_texture_map() const {return texture_map;}
	inline void set_texture_map(Image* value) {
		if(texture_map) delete texture_map;
		texture_map = value;
	}
};
