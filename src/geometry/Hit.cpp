#include "Hit.h"

Hit::Hit()
{

}

Hit::Hit(Intersectable* i, float t, Material* m)
	: intersectable(i),
	  t(t),
	  material(m)
{

}
