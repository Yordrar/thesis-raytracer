#include "RenderManager.h"

#include <math/Vector3.h>

#include <geometry/Camera.h>
#include <geometry/Scene.h>
#include <geometry/Sphere.h>
#include <geometry/Scatterer.h>
#include <geometry/mesh/Mesh.h>
#include <geometry/mesh/MeshImporter.h>
#include <geometry/light/PointLight.h>
#include <geometry/light/DirectionalLight.h>

#include <material/Lambertian.h>
#include <material/Metal.h>
#include <material/Dielectric.h>
#include <material/Emissive.h>
#include <material/RefractiveLambertian.h>

#include <renderer/CPURenderer.h>
#include <renderer/EditModeRenderer.h>

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{
	cam = new Camera();
	cam->translate_global(0, 0, 3);
	escena.set_camera(cam);

	auto meshes = MeshImporter::import_from_file("C:\\Users\\juana\\Desktop\\test.obj");
	std::vector<Intersectable*> intersectables;
	for(Mesh* m : meshes) {
		intersectables.push_back(dynamic_cast<Intersectable*>(m));
	}
	escena.set_intersectables(intersectables);
	//m->set_material(new Lambertian(Vector3(255)));
	//m->get_material()->set_texture_map(new Image("C:\\Users\\juana\\Desktop\\cow_texture.png"));
	//m->get_material()->set_normal_map(new Image("C:\\Users\\juana\\Desktop\\s76weapon_normal.png"));
	//escena.add_intersectable(m);

	PointLight* l1 = new PointLight(Vector3(128, 64, 32), 4);
	PointLight* l2 = new PointLight(Vector3(32, 64, 128), 4);
	PointLight* l3 = new PointLight(Vector3(255, 255, 255), 5);
	DirectionalLight* l4 = new DirectionalLight(Vector3(255));
	l1->set_position(Vector3(2, 0, 1));
	l2->set_position(Vector3(-2, 0, 1));
	l3->set_position(Vector3(0, 1, 2));
	//escena.add_emitter(l1);
	//escena.add_emitter(l2);
	//escena.add_emitter(l3);
	escena.add_emitter(l4);

	Sphere* s = new Sphere(Vector3(0, -100.5f, -1), 100);
	s->set_material(new Lambertian(Vector3(128)));
	Sphere* s1 = new Sphere(Vector3(0, 2, 0), 0.5f);
	s1->set_material(new Emissive(Vector3(255)));
	//s1->get_material()->set_texture_map(new Image("C:\\Users\\juana\\Desktop\\brick_diffuse.bmp"));
	//s1->get_material()->set_normal_map(new Image("C:\\Users\\juana\\Desktop\\brick_normal.bmp"));
	Sphere* s2 = new Sphere(Vector3(1.0f, 0, -1.0f), 0.5f);
	s2->set_material(new Dielectric(Vector3(255), 1.5f));
	//escena.add_intersectable(s);
	//escena.add_intersectable(s1);
	//escena.add_intersectable(s2);
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

Image RenderManager::render_preview(int width, int height)
{
	cam->set_width_and_height(width, height);

	return EditModeRenderer::render(escena);
}

Image RenderManager::render(int width, int height, int n_samples)
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

void RenderManager::move_camera(MOVE_DIRECTION direction)
{
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

void RenderManager::rotate_camera(float x0, float y0, float x1, float y1)
{
	Quaternion rotation_pitch = Quaternion::create_rotation(-(y1-y0), cam->get_right());
	Quaternion rotation_yaw = Quaternion::create_rotation(-(x1-x0), Vector3(0, 1, 0));
	Quaternion rotation_combined = rotation_yaw*rotation_pitch;
	cam->rotate(rotation_combined);
	escena.set_camera(cam);
}

QImage RenderManager::image_to_qimage(Image* img) const {
	if(img == nullptr) {
		QImage qimg(1, 1, QImage::Format_RGB888);
		qimg.fill(0);
		return qimg;
	}

	QImage qimg(img->get_width(), img->get_height(), QImage::Format_RGB888);
	for(int j = 0; j < qimg.height(); j++) {
		for(int i = 0; i < qimg.width(); i++) {
			Vector3 color = img->get_pixel_color(i, j) * 255.99f;
			qimg.setPixelColor(i, j, QColor(static_cast<int>(color.get_x()),
											static_cast<int>(color.get_y()),
											static_cast<int>(color.get_z())));
		}
	}
	return qimg;
}

QImage RenderManager::get_texture_map() const
{
	Image* img = dynamic_cast<Scatterer*>(entity_selected)->get_material()->get_texture_map();
	return image_to_qimage(img);
}

QImage RenderManager::get_normal_map() const
{
	Image* img = dynamic_cast<Scatterer*>(entity_selected)->get_material()->get_normal_map();
	return image_to_qimage(img);
}

Entity* RenderManager::get_selection(int x, int y)
{
	BVH hierarchy(escena.get_intersectables());
	return cam->get_object(x, y, hierarchy);
}
