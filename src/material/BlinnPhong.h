#pragma once

#include "Material.h"

#include <image/Image.h>

class BlinnPhong : public Material
{
public:
	BlinnPhong(const Vector3& albedo = Vector3(128, 128, 128), float shininess = 50.0f);
	BlinnPhong(float r, float g, float b, float shininess = 50.0f);
	~BlinnPhong() override;

	Ray scatter(const Ray& ray, float t, const Vector3& normal) override;

	Vector3 get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const override;

	Vector3 get_ambient_albedo() const;
	void set_ambient_albedo(const Vector3& value);

	Vector3 get_specular_albedo() const;
	void set_specular_albedo(const Vector3& value);

	void set_texture_map(Image *value) override;

private:
	float shininess;
	Vector3 ambient_albedo, specular_albedo;
};
