#include "Scatterer.h"

Scatterer::Scatterer(Material* m, Image* i)
	: material(m),
	  texture_map(i)
{

}

Scatterer::~Scatterer()
{
	if(material) delete material;
	if(texture_map) delete texture_map;
}
