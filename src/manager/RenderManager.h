#pragma once

#include <QPixmap>

class RenderManager
{
public:
	~RenderManager();
	static RenderManager* get_manager();

	QPixmap render(int width, int height, int n_samples);

private:
	RenderManager();
	static RenderManager* instance;
};

