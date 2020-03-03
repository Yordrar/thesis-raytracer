#include "Intersectable.h"

#include <material/Lambertian.h>

Intersectable::Intersectable()
{
	material = new Lambertian();
}

Intersectable::Intersectable(Material* m)
{
	material = m;
}

Intersectable::~Intersectable()
{
	delete material;
}
