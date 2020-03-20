#pragma once

#include <image/Framebuffer.h>
#include <geometry/Scene.h>

class RenderManager
{
public:
	~RenderManager();
	static RenderManager* get_manager();

	Framebuffer render_preview(int width, int height);
	Framebuffer render(int width, int height, int n_samples);
	void move_camera(float delta_x, float delta_y, float delta_z);
	void rotate_camera(float x0, float y0, float x1, float y1);

private:
	RenderManager();
	static RenderManager* instance;

	Scene escena;
	Camera* cam;
};

