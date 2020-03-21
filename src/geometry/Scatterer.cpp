#include "Scatterer.h"

Scatterer::Scatterer(Material* m)
{
	material = m;
}

Scatterer::~Scatterer()
{
	if(material) delete material;
}
