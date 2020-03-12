#include "Hit.h"

Hit::Hit(bool hit, Material* m, Vector3 n, float t)
	: hit(hit),
	  material(m),
	  normal(n),
	  t(t)
{

}
