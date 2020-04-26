#pragma once

#include <image/Image.h>
#include <geometry/Scene.h>

#include <QImage>
#include <QString>

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

	QImage get_texture_map() const;
	inline void set_texture_map(QString filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_texture_map(new Image(filename.toUtf8().constData()));
	}

	QImage get_normal_map() const;
	inline void set_normal_map(QString filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_normal_map(new Image(filename.toUtf8().constData()));
	}

	enum class MATERIAL_TYPE {
		LAMBERTIAN,
		BLINNPHONG,
		METAL,
		DIELECTRIC,
		EMISSIVE,
		REFRACTIVE_LAMBERTIAN,
	};
	void set_material(MATERIAL_TYPE m) const;

	void make_selection(int x, int y);

	bool set_position_entity_selected(float x, float y, float z);
	bool set_orientation_entity_selected(float euler_x, float euler_y, float euler_z);

private:
	RenderManager();
	static RenderManager* instance;

	Entity* entity_selected = nullptr;

	QImage image_to_qimage(Image* img) const;
};

