#pragma once

#include <geometry/Scene.h>

#include <image/Framebuffer.h>

class EditModeRenderer
{
public:
	EditModeRenderer() = delete;
	~EditModeRenderer() = delete;

	static Framebuffer render(const Scene& scene);
};

