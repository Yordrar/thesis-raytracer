#include "Ray.h"

Ray::Ray(Vector3 direction)
	: origin(Vector3(0, 0, 0)),
	  direction(direction.unit()),
	  direction_inverse(1.0f/direction)
{

}

Ray::Ray(Vector3 origin, Vector3 direction)
	: origin(origin),
	  direction(direction.unit()),
	  direction_inverse(1.0f/direction)
{

}
