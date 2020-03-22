#include "Hit.h"

Hit::Hit(bool hit, Material* m, Vector3 n, float t, Vector3 uv, Image* texture)
	: hit(hit),
	  material(m),
	  normal(n),
	  t(t),
	  uv(uv),
	  texture(texture)
{

}
