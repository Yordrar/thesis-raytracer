#include "SceneManager.h"

#include <geometry/light/DirectionalLight.h>
#include <geometry/light/PointLight.h>
#include <geometry/mesh/MeshImporter.h>

#include <material/Dielectric.h>
#include <material/Emissive.h>
#include <material/Lambertian.h>

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
	camera = new Camera;
	camera->translate_global(-0.1f, 1, -2);
	camera->rotate(-20, 0, 0);

	scene = new Scene;

	auto meshes = MeshImporter::import_from_file("C:\\Users\\juana\\Desktop\\test3.obj");
	std::vector<Intersectable*> intersectables;
	for(Mesh* m : meshes) {
		intersectables.push_back(dynamic_cast<Intersectable*>(m));
	}
	scene->set_intersectables(intersectables);

	PointLight* l1 = new PointLight(Vector3(128, 64, 32), 80);
	PointLight* l2 = new PointLight(Vector3(32, 64, 128), 80);
	PointLight* l3 = new PointLight(Vector3(255, 255, 255), 80);
	DirectionalLight* l4 = new DirectionalLight(Vector3(255));
	PointLight* l5 = new PointLight(Vector3(255, 255, 255), 80);
	l1->set_position(Vector3(-2, 1, -2));
	l2->set_position(Vector3(-2, 1, -8));
	l3->set_position(Vector3(-2, 1, -14));
	l5->set_position(Vector3(3, 3, -1));
	//escena.add_emitter(l1);
	//escena.add_emitter(l2);
	//escena.add_emitter(l3);
	scene->add_emitter(l4);
	//escena.add_emitter(l5);

	Sphere* s = new Sphere(Vector3(0, -100.5f, -1), 100);
	s->set_material(new Lambertian(Vector3(128)));
	Sphere* s1 = new Sphere(Vector3(0, 2, 0), 0.5f);
	s1->set_material(new Emissive(Vector3(255)));
	//s1->get_material()->set_texture_map(new Image("C:\\Users\\juana\\Desktop\\brick_diffuse.bmp"));
	//s1->get_material()->set_normal_map(new Image("C:\\Users\\juana\\Desktop\\brick_normal.bmp"));
	Sphere* s2 = new Sphere(Vector3(0, 0, -2.0f), 0.5f);
	s2->set_material(new Dielectric(Vector3(255), 1.5f));
	//escena.add_intersectable(s);
	//escena.add_intersectable(s1);
	//escena.add_intersectable(s2);
}

SceneManager::~SceneManager()
{

}

SceneManager* SceneManager::get_manager()
{
	if(!instance) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::move_camera(MOVE_DIRECTION direction)
{
	auto* cam = SceneManager::get_manager()->get_camera();
	if(direction == MOVE_DIRECTION::FRONT) {
		cam->translate_global(0, 0, -move_velocity);
	}
	else if(direction == MOVE_DIRECTION::LEFT) {
		cam->translate_global(-move_velocity, 0, 0);
	}
	else if(direction == MOVE_DIRECTION::BACK) {
		cam->translate_global(0, 0, move_velocity);
	}
	else if(direction == MOVE_DIRECTION::RIGHT) {
		cam->translate_global(move_velocity, 0, 0);
	}
	else if(direction == MOVE_DIRECTION::UP) {
		cam->translate_global(0, move_velocity, 0);
	}
	else if(direction == MOVE_DIRECTION::DOWN) {
		cam->translate_global(0, -move_velocity, 0);
	}
}

void SceneManager::rotate_camera(float x0, float y0, float x1, float y1)
{
	auto* cam = SceneManager::get_manager()->get_camera();
	Quaternion rotation_pitch = Quaternion::create_rotation(-(y1-y0), cam->get_right());
	Quaternion rotation_yaw = Quaternion::create_rotation(-(x1-x0), Vector3(0, 1, 0));
	Quaternion rotation_combined = rotation_yaw*rotation_pitch;
	cam->rotate(rotation_combined);
}

void SceneManager::load_from_file(std::string filename)
{
	auto meshes = MeshImporter::import_from_file(filename);
	std::vector<Intersectable*> intersectables;
	for(Mesh* m : meshes) {
		intersectables.push_back(dynamic_cast<Intersectable*>(m));
	}
	scene->set_intersectables(intersectables);
}

void SceneManager::add_from_file(std::string filename)
{
	auto meshes = MeshImporter::import_from_file(filename);
	for(Mesh* m : meshes) {
		scene->add_intersectable(dynamic_cast<Intersectable*>(m));
	}
}
