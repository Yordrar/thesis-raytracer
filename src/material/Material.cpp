#include "Material.h"

Material::Material(const Vector3& albedo)
{
	this->albedo = albedo/255.99f;
}

Material::Material(float r, float g, float b)
{
	albedo = Vector3(r, g, b)/255.99f;
}

Material::~Material()
{

}
