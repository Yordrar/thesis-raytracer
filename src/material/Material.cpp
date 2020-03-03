#include "Material.h"

Material::Material(Vector3 albedo)
{
	this->albedo = albedo;
}

Material::Material(float r, float g, float b)
{
	albedo = Vector3(r, g, b);
}

Material::~Material()
{

}
