#include "RenderManager.h"

#include <math/Vector3.h>

#include <geometry/Camera.h>
#include <geometry/Scene.h>
#include <geometry/Sphere.h>
#include <geometry/mesh/Mesh.h>
#include <geometry/mesh/MeshImporter.h>
#include <geometry/light/PointLight.h>

#include <material/Lambertian.h>
#include <material/Metal.h>
#include <material/Dielectric.h>

#include <renderer/CPURenderer.h>
#include <renderer/EditModeRenderer.h>

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{
	cam = new Camera();
	cam->translate(0, 0, 4);
	escena.set_camera(cam);

	Mesh* m = MeshImporter::import_from_file("C:\\Users\\juana\\Desktop\\suzanne.obj");
	m->set_material(new Lambertian());
	escena.add_intersectable(m);

	PointLight* l1 = new PointLight(Vector3(128, 64, 32), 2);
	PointLight* l2 = new PointLight(Vector3(32, 64, 128), 2);
	PointLight* l3 = new PointLight(Vector3(255, 255, 255), 1);
	l1->set_position(Vector3(2, 0, 1));
	l2->set_position(Vector3(-2, 0, 1));
	l3->set_position(Vector3(0, 1, 2));
	escena.add_emitter(l1);
	escena.add_emitter(l2);
	//escena.add_emitter(l3);

	Sphere* s = new Sphere(Vector3(0, -101, -1), 100);
	s->set_material(new Lambertian());
	escena.add_intersectable(s);
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

Framebuffer RenderManager::render_preview(int width, int height)
{
	cam->set_width_and_height(width, height);

	return EditModeRenderer::render(escena);
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

	cam->set_width_and_height(width, height);

	return CPURenderer::render(escena, n_samples);
}

void RenderManager::move_camera(float delta_x, float delta_y, float delta_z)
{
	cam->translate(delta_x, delta_y, delta_z);
}

void RenderManager::rotate_camera(float x0, float y0, float x1, float y1)
{
	Quaternion rotation_pitch = Quaternion::create_rotation(-(y1-y0), cam->get_right());
	Quaternion rotation_yaw = Quaternion::create_rotation(-(x1-x0), Vector3(0, 1, 0));
	Quaternion rotation_combined = rotation_yaw*rotation_pitch;
	cam->rotate(rotation_combined);
	escena.set_camera(cam);
}
