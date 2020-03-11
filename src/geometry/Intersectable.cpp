#include "Intersectable.h"

#include <material/Lambertian.h>

Intersectable::Intersectable()
{
	material = nullptr;
}

Intersectable::Intersectable(Material* m)
{
	material = m;
}

Intersectable::~Intersectable()
{
	if(material) delete material;
}
