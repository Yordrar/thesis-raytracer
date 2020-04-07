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
	Image render(int width, int height, int n_samples);

	enum class MOVE_DIRECTION {
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		UP,
		DOWN,
	};
	inline void set_move_velocity(float value) {move_velocity = value;}
	void move_camera(MOVE_DIRECTION direction);
	void rotate_camera(float x0, float y0, float x1, float y1);

	inline bool is_there_entity_selected() const {return entity_selected != nullptr;}
	inline void set_entity_selected(Entity* e) {entity_selected = e;}

	QImage get_texture_map() const;
	inline void set_texture_map(QString filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_texture_map(new Image(filename.toUtf8().constData()));
	}

	QImage get_normal_map() const;
	inline void set_normal_map(QString filename) const {
		dynamic_cast<Scatterer*>(entity_selected)->get_material()->set_normal_map(new Image(filename.toUtf8().constData()));
	}



	Entity* get_selection(int x, int y);

private:
	RenderManager();
	static RenderManager* instance;

	Scene escena;
	Camera* cam;
	float move_velocity = 0.1f;
	Entity* entity_selected;

	QImage image_to_qimage(Image* img) const;
};

