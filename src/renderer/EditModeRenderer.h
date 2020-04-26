#pragma once

#include <geometry/Scene.h>

#include <image/Image.h>

class EditModeRenderer
{
public:
	EditModeRenderer() = delete;
	~EditModeRenderer() = delete;

	static Image render(const Scene* scene, Camera* camera, Entity* entity_selected);
};

