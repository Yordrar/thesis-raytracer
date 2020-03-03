#include "RenderManager.h"

#include <math/Vector3.h>
#include <geometry/Camera.h>
#include <geometry/Scene.h>
#include <geometry/Sphere.h>

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{

}

RenderManager::~RenderManager()
{

}

RenderManager* RenderManager::get_manager()
{
	if(!instance) {
		instance = new RenderManager;
	}
	return instance;
}

QPixmap RenderManager::render(int width, int height)
{
	Sphere s1(Vector3(0, 0, -1.0f), 0.5f);
	Sphere s2(Vector3(0, -100-s1.get_radius(), -1), 100);
	Camera c(Vector3(), Vector3(0, 0, -1), width, height);
	Scene escena{&s1, &s2};
	escena.set_camera(c);

	return escena.render();
}
