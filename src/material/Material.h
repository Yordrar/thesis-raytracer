#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>
#include <image/Image.h>
#include <vector>

class Material
{
public:
	Material(const Vector3& albedo = Vector3(128, 128, 128));
	Material(float r, float g, float b);
	virtual ~Material() = 0;

	inline Vector3 get_albedo() const {return albedo;}
	inline void set_albedo(const Vector3& new_albedo) {albedo = new_albedo;}

	inline Image* get_texture_map() const {return texture_map;}
	virtual inline void set_texture_map(Image* value) {
		if(texture_map) delete texture_map;
		texture_map = value;
	}

	inline Image* get_normal_map() const {return normal_map;}
	inline void set_normal_map(Image* value) {
		if(normal_map) delete normal_map;
		normal_map = value;
	}

	virtual Ray scatter(const Ray& ray, float t, const Vector3& normal) = 0;
	virtual Vector3 get_emission_color(const Ray& ray, float t, const Vector3& normal);

	virtual Vector3 get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const;
	virtual Vector3 get_normal(const Vector3& uv) const;

	virtual inline bool is_affected_by_shadow_rays() const {return true;}

protected:
	Vector3 albedo;
	Image* texture_map = nullptr;
	Image* normal_map = nullptr;
};

