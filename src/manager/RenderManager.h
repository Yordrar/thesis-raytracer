#pragma once

#include <image/Framebuffer.h>

class RenderManager
{
public:
	~RenderManager();
	static RenderManager* get_manager();

	Framebuffer render(int width, int height, int n_samples);

private:
	RenderManager();
	static RenderManager* instance;
};

