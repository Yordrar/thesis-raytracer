#include "OptionsManager.h"
#include "SceneManager.h"

#include <geometry/light/AreaLight.h>
#include <geometry/light/DirectionalLight.h>
#include <geometry/light/PointLight.h>
#include <geometry/mesh/MeshImporter.h>

#include <material/BlinnPhong.h>
#include <material/Dielectric.h>
#include <material/Emissive.h>
#include <material/Lambertian.h>
#include <material/Metal.h>

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
    camera = new Camera;
	camera->translate_global(0, 0, 1);

	scene = new Scene;
/*
	auto meshes = MeshImporter::import_from_file("C:\\Users\\juana\\Desktop\\mediano.obj");
	std::vector<Intersectable*> intersectables;
	for(Mesh* m : meshes) {
		intersectables.push_back(dynamic_cast<Intersectable*>(m));
	}
	scene->set_intersectables(intersectables);
*/

	PointLight* l1 = new PointLight(Vector3(128, 64, 32), 80);
	PointLight* l2 = new PointLight(Vector3(32, 64, 128), 80);
	PointLight* l3 = new PointLight(Vector3(255, 255, 255), 80);
	DirectionalLight* l4 = new DirectionalLight(Vector3(255));
	PointLight* l5 = new PointLight(Vector3(255, 255, 255), 80);
	//AreaLight* l6 = new AreaLight(Vector3(248, 222, 126), 10);
	AreaLight* l6 = new AreaLight(Vector3(255), 10);
	l1->set_position(Vector3(-2, 1, -2));
	l2->set_position(Vector3(-2, 1, -8));
	l3->set_position(Vector3(-2, 1, -14));
	l5->set_position(Vector3(3, 3, -1));
	l6->set_position(Vector3(0, 3, 0));
	l6->rotate_global(90, 0, 0);
	//scene->add_emitter(l1);
	//scene->add_emitter(l2);
	//scene->add_emitter(l3);
	scene->add_emitter(l4);
	//scene->add_emitter(l5);
	//scene->add_emitter(l6);

	Sphere* s = new Sphere(Vector3(0, -100.5f, -1), 100);
	s->set_material(new Lambertian(Vector3(128)));
    Sphere* s1 = new Sphere(Vector3(1, 0, -1), 0.5f);
	s1->set_material(new Metal());
	//s1->get_material()->set_texture_map(new Image("C:\\Users\\juana\\Desktop\\brick_diffuse.bmp"));
    //s1->get_material()->set_normal_map(new Image("C:\\Users\\juana\\Desktop\\brick_normal.bmp"));
    Sphere* s2 = new Sphere(Vector3(-1, 0, -1), 0.5f);
	s2->set_material(new Dielectric());
	Sphere* s3 = new Sphere(Vector3(0, 0, -1), 0.5f);
    s3->set_material(new Lambertian(Vector3(128)));
	//scene->add_intersectable(s);
	//scene->add_intersectable(s1);
	//scene->add_intersectable(s2);
	//scene->add_intersectable(s3);

    /*
    Sphere* m6 = new Sphere(Vector3(-50, 0, 0), 5);m6->set_material(new BlinnPhong());m6->get_material()->set_reflectance(0);m6->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m5 = new Sphere(Vector3(-40, 0, 0), 5);m5->set_material(new BlinnPhong());m5->get_material()->set_reflectance(0.1f);m5->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m4 = new Sphere(Vector3(-30, 0, 0), 5);m4->set_material(new BlinnPhong());m4->get_material()->set_reflectance(0.2f);m4->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m3 = new Sphere(Vector3(-20, 0, 0), 5);m3->set_material(new BlinnPhong());m3->get_material()->set_reflectance(0.3f);m3->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m2 = new Sphere(Vector3(-10, 0, 0), 5);m2->set_material(new BlinnPhong());m2->get_material()->set_reflectance(0.4f);m2->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m1 = new Sphere(Vector3(), 5);m1->set_material(new BlinnPhong());m1->get_material()->set_reflectance(0.5f);m1->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m7 = new Sphere(Vector3(10, 0, 0), 5);m7->set_material(new BlinnPhong());m7->get_material()->set_reflectance(0.6f);m7->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m8 = new Sphere(Vector3(20, 0, 0), 5);m8->set_material(new BlinnPhong());m8->get_material()->set_reflectance(0.7f);m8->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m9 = new Sphere(Vector3(30, 0, 0), 5);m9->set_material(new BlinnPhong());m9->get_material()->set_reflectance(0.8f);m9->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m10 = new Sphere(Vector3(40, 0, 0), 5);m10->set_material(new BlinnPhong());m10->get_material()->set_reflectance(0.9f);m10->get_material()->set_albedo(Vector3(1,0,0));
    Sphere* m11 = new Sphere(Vector3(50, 0, 0), 5);m11->set_material(new BlinnPhong());m11->get_material()->set_reflectance(1);m11->get_material()->set_albedo(Vector3(1,0,0));
    scene->add_intersectable(m1);
    scene->add_intersectable(m2);
    scene->add_intersectable(m3);
    scene->add_intersectable(m4);
    scene->add_intersectable(m5);
    scene->add_intersectable(m6);
    scene->add_intersectable(m7);
    scene->add_intersectable(m8);
    scene->add_intersectable(m9);
    scene->add_intersectable(m10);
    scene->add_intersectable(m11);
    */
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
		camera->orbital_anchor_zoom(-zoom_velocity);
	}
	else if(direction == MOVE_DIRECTION::BACK) {
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
