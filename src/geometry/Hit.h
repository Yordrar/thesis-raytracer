#pragma once

#include <geometry/Scatterer.h>

#include <material/Material.h>

class Hit
{
public:
	Hit(bool hit = false,
		Material* m = nullptr,
		Vector3 normal = Vector3(),
		float t = 0.0f,
		Vector3 uv = Vector3(-1),
		Vector3 tangent = Vector3(),
		Vector3 bitangent = Vector3(),
		Vector3 position = Vector3());

	inline bool is_hit() const {return hit;}

	inline Material* get_material() const {return material;}
	inline void set_material(Material* m) {material = m;}

	inline Vector3 get_normal() const {return normal;}
	inline void set_normal(const Vector3& n) {normal = n;}

	inline float get_t() const {return t;}
	inline void set_t(float new_t) {t = new_t;}

	inline Vector3 get_uv() const {return uv;}
	inline void set_uv(const Vector3& value) {uv = value;}

	inline Vector3 get_tangent() const {return tangent;}
	inline void set_tangent(const Vector3& value) {tangent = value;}

	inline Vector3 get_bitangent() const {return bitangent;}
	inline void set_bitangent(const Vector3& value) {bitangent = value;}

	inline Vector3 get_entity_position() const {return entity_position;}
	inline void set_entity_position(const Vector3& value) {entity_position = value;}

private:
	bool hit;
	Material* material;
	Vector3 normal;
	float t;
	Vector3 uv;
	Vector3 tangent;
	Vector3 bitangent;
	Vector3 entity_position;
};

