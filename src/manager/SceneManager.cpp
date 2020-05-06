#include "OptionsManager.h"
#include "SceneManager.h"

#include <geometry/light/AreaLight.h>
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
	AreaLight* l6 = new AreaLight(Vector3(255), 40);
	l1->set_position(Vector3(-2, 1, -2));
	l2->set_position(Vector3(-2, 1, -8));
	l3->set_position(Vector3(-2, 1, -14));
	l5->set_position(Vector3(3, 3, -1));
	l6->set_position(Vector3(0, 2, -4));
	l6->rotate_global(90, 0, 0);
	//scene->add_emitter(l1);
	//scene->add_emitter(l2);
	//scene->add_emitter(l3);
	scene->add_emitter(l4);
	//scene->add_emitter(l5);
	//scene->add_emitter(l6);

	Sphere* s = new Sphere(Vector3(0, -100.5f, -1), 100);
	s->set_material(new Lambertian(Vector3(128)));
	Sphere* s1 = new Sphere(Vector3(0, 2, 0), 0.5f);
	s1->set_material(new Emissive(Vector3(255)));
	//s1->get_material()->set_texture_map(new Image("C:\\Users\\juana\\Desktop\\brick_diffuse.bmp"));
	//s1->get_material()->set_normal_map(new Image("C:\\Users\\juana\\Desktop\\brick_normal.bmp"));
	Sphere* s2 = new Sphere(Vector3(1, 0.5f, -3), 0.5f);
	s2->set_material(new Dielectric(Vector3(255)));
	//escena.add_intersectable(s);
	//escena.add_intersectable(s1);
	scene->add_intersectable(s2);
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
	float move_velocity = OptionsManager::get_manager()->getCamera_move_velocity();
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

void SceneManager::rotate_camera_fps(float x0, float y0, float x1, float y1)
{
	Quaternion rotation_pitch = Quaternion::create_rotation(-(y1-y0)*0.5f, camera->get_right());
	Quaternion rotation_yaw = Quaternion::create_rotation(-(x1-x0)*0.5f, Vector3(0, 1, 0));
	Quaternion rotation_combined = rotation_yaw*rotation_pitch;
	camera->rotate(rotation_combined);
}

void SceneManager::rotate_camera_orbital(float x0, float y0, float x1, float y1)
{
	camera->rotate_orbital(-(y1-y0)*0.5f, -(x1-x0)*0.5f);
}

void SceneManager::orbital_anchor_zoom(MOVE_DIRECTION direction)
{
	float zoom_velocity = OptionsManager::get_manager()->getCamera_zoom_velocity();
	if(direction == MOVE_DIRECTION::FRONT) {
		move_camera(SceneManager::MOVE_DIRECTION::FRONT);
		camera->orbital_anchor_zoom(-zoom_velocity);
	}
	else if(direction == MOVE_DIRECTION::BACK) {
		move_camera(SceneManager::MOVE_DIRECTION::BACK);
		camera->orbital_anchor_zoom(zoom_velocity);
	}
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
