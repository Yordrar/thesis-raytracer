#pragma once

#include <geometry/Scatterer.h>

#include <material/Material.h>

class Hit
{
public:
	Hit(bool hit = false, Material* m = nullptr, Vector3 normal = Vector3(), float t = 0.0f);

	inline bool is_hit() const {return hit;}
	inline float get_t() const {return t;}
	inline Material* get_material() const {return material;}
	inline Vector3 get_normal() const {return normal;}

	inline void set_t(float new_t) {t = new_t;}
	inline void set_material(Material* m) {material = m;}
	inline void set_normal(const Vector3& n) {normal = n;}

private:
	bool hit;
	Material* material;
	Vector3 normal;
	float t;
};

