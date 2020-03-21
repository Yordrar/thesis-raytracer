#include "Emitter.h"

Emitter::Emitter(Vector3 albedo, float intensity)
	: albedo(albedo/255.99f),
	  intensity(intensity)
{

}

Emitter::~Emitter()
{

}
