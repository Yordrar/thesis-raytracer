#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>
#include <geometry/Entity.h>

class Material
{
public:
	Material(const Vector3& albedo = Vector3(128, 128, 128));
	Material(float r, float g, float b);
	virtual ~Material() = 0;

	inline Vector3 get_albedo() const {return albedo;}
	inline void set_albedo(const Vector3& new_albedo) {albedo = new_albedo;}

	virtual Ray scatter(const Ray& ray, float t, const Vector3& normal) = 0;

protected:
	Vector3 albedo;
};

