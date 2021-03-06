#pragma once

#include <image/Image.h>
#include <geometry/Scene.h>
#include <renderer/CPURenderer.h>

#include <string>

class RenderManager
{
public:
	~RenderManager();
	static RenderManager* get_manager();

	Image render_preview(int width, int height);
	void render(int width, int height, int n_samples);
	Image* get_rendered_image();
	bool is_render_finished();
	void finish_render();

	inline bool is_there_entity_selected() const {return entity_selected != nullptr;}
	inline Entity* get_entity_selected() {return entity_selected;}

	Image* get_texture_map() const;
	inline void set_texture_map(std::string filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_texture_map(new Image(filename));
	}

	Image* get_normal_map() const;
	inline void set_normal_map(std::string filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_normal_map(new Image(filename));
	}

	Image* get_roughness_map() const;
	inline void set_roughness_map(std::string filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_roughness_map(new Image(filename));
	}

	Image* get_metallicity_map() const;
	inline void set_metallicity_map(std::string filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_metallicity_map(new Image(filename));
	}

	Image* get_ao_map() const;
	inline void set_ao_map(std::string filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_ao_map(new Image(filename));
	}

	enum class MATERIAL_TYPE {
		NONE,
		LAMBERTIAN,
		BLINNPHONG,
		METAL,
		DIELECTRIC,
		EMISSIVE,
		REFRACTIVE_LAMBERTIAN,
	};
	void set_material(MATERIAL_TYPE m) const;
	MATERIAL_TYPE get_material() const;

	void make_selection(int x, int y);

	bool set_position_entity_selected(float x, float y, float z);
	bool set_orientation_entity_selected(float euler_x, float euler_y, float euler_z);

	inline Vector3 get_albedo() const {return dynamic_cast<Scatterer*>(entity_selected)->get_material()->get_albedo();}
	inline void set_albedo(float albedo_x, float albedo_y, float albedo_z) {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_albedo(Vector3(albedo_x, albedo_y, albedo_z) / 255);
	}

	inline float get_roughness() const {return dynamic_cast<Scatterer*>(entity_selected)->get_material()->get_roughness();}
	inline void set_roughness(float value) {dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_roughness(value);}

	inline float get_metallicity() const {return dynamic_cast<Scatterer*>(entity_selected)->get_material()->get_metallicity();}
	inline void set_metallicity(float value) {dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_metallicity(value);}

	inline float get_reflectance() const {return dynamic_cast<Scatterer*>(entity_selected)->get_material()->get_reflectance();}
	inline void set_reflectance(float value) {dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_reflectance(value);}

	float get_intensity() const;
	void set_intensity(float value);

	float get_refraction_index() const;
	void set_refraction_index(float value);

	int get_samples_rendered() const {return CPURenderer::get_renderer()->get_samples_rendered();}

	inline Image* get_environment_map() const {return environment_map;}
	inline void set_environment_map(std::string filename) {
		if(environment_map) delete environment_map;
		environment_map = new Image(filename);
	}

private:
	RenderManager();
	static RenderManager* instance;

	Entity* entity_selected = nullptr;
	Image* environment_map = nullptr;
};

