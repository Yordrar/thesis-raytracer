#include "Scatterer.h"

Scatterer::Scatterer()
{
	material = nullptr;
}

Scatterer::Scatterer(Material* m)
{
	material = m;
}

Scatterer::~Scatterer()
{
	if(material) delete material;
}
