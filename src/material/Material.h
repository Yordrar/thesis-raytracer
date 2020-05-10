#pragma once

#include <math/Vector3.h>
#include <math/Ray.h>
#include <image/Image.h>
#include <vector>

class Material
{
public:
	Material(const Vector3& albedo = Vector3(255));
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

	inline Image* get_roughness_map() const {return roughness_map;}
	virtual inline void set_roughness_map(Image* value) {
		if(roughness_map) delete roughness_map;
		roughness_map = value;
	}

	inline Image* get_metallicity_map() const {return metallicity_map;}
	inline void set_metallicity_map(Image* value) {
		if(metallicity_map) delete metallicity_map;
		metallicity_map = value;
	}

	inline Image* get_ao_map() const {return ao_map;}
	virtual inline void set_ao_map(Image* value) {
		if(ao_map) delete ao_map;
		ao_map = value;
	}

	virtual Ray scatter(const Ray& ray, float t, const Vector3& normal, float roughness, float metallicity) = 0;
	virtual Vector3 get_emission_color(const Ray& ray, float t, const Vector3& normal);

	virtual Vector3 get_color(const Vector3& uv, const Vector3& normal, const std::vector<Vector3>& light_vectors, const Vector3& view_vector) const;
	virtual Vector3 get_normal(const Vector3& uv) const;
	float get_roughness_from_map(const Vector3& uv) const;
	float get_metallicity_from_map(const Vector3& uv) const;

	virtual inline bool is_affected_by_shadow_rays() const {return true;}

	inline float get_roughness() const {return roughness;}
	inline void set_roughness(float value) {roughness = Math::Clampf(value, 0, 1);}

	inline float get_metallicity() const {return metallicity;}
	inline void set_metallicity(float value) {metallicity = Math::Clampf(value, 0, 1);}

	inline float get_reflectance() const {return reflectance;}
	inline void set_reflectance(float value) {reflectance = Math::Clampf(value, 0, 1);}

protected:
	Vector3 albedo;
	float  roughness, metallicity, reflectance;
	Image* texture_map = nullptr;
	Image* normal_map = nullptr;
	Image* roughness_map = nullptr;
	Image* metallicity_map = nullptr;
	Image* ao_map = nullptr;
};

