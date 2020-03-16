#pragma once

#include <material/Material.h>

class Scatterer
{
protected:
	Scatterer();
	Scatterer(Material* m);
	virtual ~Scatterer() = 0;

	Material* material;

public:
	inline Material* get_material() const {return material;}
	inline void set_material(Material* m) {
		if(material) delete material;
		material = m;
	}
};
