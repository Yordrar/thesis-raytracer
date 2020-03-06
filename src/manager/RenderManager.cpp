#include "RenderManager.h"

#include <math/Vector3.h>

#include <geometry/Camera.h>
#include <geometry/Scene.h>
#include <geometry/Sphere.h>

#include <material/Lambertian.h>
#include <material/Metal.h>
#include <material/Dielectric.h>

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

QPixmap RenderManager::render(int width, int height, int n_samples)
{
	Sphere s1(Vector3(0, 0, -1.0f), 0.5f);
	s1.set_material(new Lambertian(Vector3(10, 60, 255)));

	Sphere s2(Vector3(0, -100-s1.get_radius(), -1), 100);

	Sphere s3(Vector3(1.0f, 0, -1.0f), 0.5f);
	s3.set_material(new Metal(Vector3(128, 128, 128), 0.5f));

	Sphere s4(Vector3(-1.0f, 0, -1.0f), 0.5f);
	s4.set_material(new Dielectric(Vector3(255, 255, 255), 1.5f));

	Camera c(Vector3(0, 0, 0), Vector3(0, 0, -1), width, height);
	Scene escena{&s1, &s2, &s3, &s4};
	escena.set_camera(c);

	/*
	Sphere s5(Vector3(-1.0f, 0, -1.0f), -0.45f);
	s5.set_material(new Dielectric(Vector3(255, 255, 255), 1.5f));
	escena.add_intersectable(&s5);
	*/

	return escena.render(n_samples);
}
