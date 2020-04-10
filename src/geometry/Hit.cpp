#include "Hit.h"

Hit::Hit(bool hit, Material* m, Vector3 n, float t, Vector3 uv, Vector3 tangent, Vector3 bitangent)
	: hit(hit),
	  material(m),
	  normal(n),
	  t(t),
	  uv(uv),
	  tangent(tangent),
	  bitangent(bitangent)
{

}
