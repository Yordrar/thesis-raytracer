#pragma once

#include <geometry/Scene.h>

class SceneManager
{
public:
	~SceneManager();
	static SceneManager* get_manager();

	inline Scene* get_scene() const {return scene;}
	inline Camera* get_camera() const {return camera;}

	enum class MOVE_DIRECTION {
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		UP,
		DOWN,
	};

	void move_camera(MOVE_DIRECTION direction);
	void rotate_camera_fps(float x0, float y0, float x1, float y1);
	void rotate_camera_orbital(float x0, float y0, float x1, float y1);
	void orbital_anchor_zoom(MOVE_DIRECTION direction);

	inline void clear_scene() {scene->set_intersectables({});}
	void load_from_file(std::string filename);
	void add_from_file(std::string filename);

private:
	SceneManager();
	static SceneManager* instance;

	Scene* scene = nullptr;
	Camera* camera = nullptr;
};

