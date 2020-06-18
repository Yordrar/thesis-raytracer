#pragma once

#include "Material.h"

#include <image/Image.h>

class BlinnPhong : public Material
{
public:
	using Material::Material;
	~BlinnPhong() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity) override;

    Vector3 get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const override;

	Vector3 get_ambient_albedo() const;
	void set_ambient_albedo(const Vector3& value);

	Vector3 get_specular_albedo() const;
	void set_specular_albedo(const Vector3& value);

	void set_texture_map(Image *value) override;

private:
	Vector3 ambient_albedo = Vector3(0);
    Vector3 specular_albedo = Vector3(255);
    float fresnel(float n1, float n2, Vector3 normal, Vector3 incident);
};
