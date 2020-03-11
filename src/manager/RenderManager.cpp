#include "RenderManager.h"

#include <math/Vector3.h>

#include <geometry/Camera.h>
#include <geometry/Scene.h>
#include <geometry/Sphere.h>
#include <geometry/mesh/Mesh.h>
#include <geometry/mesh/MeshLoader.h>

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

Framebuffer RenderManager::render(int width, int height, int n_samples)
{
	/*Sphere s1(Vector3(0, 0, -1.0f), 0.5f);
	s1.set_material(new Lambertian(Vector3(10, 60, 255)));

	Sphere s2(Vector3(0, -100-s1.get_radius(), -1), 100);

	Sphere s3(Vector3(1.0f, 0, -1.0f), 0.5f);
	s3.set_material(new Metal(Vector3(128, 128, 128)));

	Sphere s4(Vector3(-1.0f, 0, -1.0f), 0.5f);
	s4.set_material(new Dielectric(Vector3(255, 255, 255), 1.5f));

	Sphere s5(Vector3(-1.0f, 0, -1.0f), -0.45f);
	s5.set_material(new Dielectric(Vector3(255, 255, 255), 1.5f));
	escena.add_intersectable(&s5);

	Scene escena{&s1, &s2, &s3, &s4};*/

	Scene escena;
	Camera c(Vector3(0, 0, 4), Vector3(0, 0, -1), width, height, 45);
	escena.set_camera(c);

	Mesh m = MeshLoader::load_from_file("C:\\Users\\juana\\Desktop\\suzanne.obj");
	m.set_material(new Lambertian());
	escena.add_intersectable(&m);

	return escena.render(n_samples);
}
