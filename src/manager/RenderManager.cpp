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
#include <material/BlinnPhong.h>

#include <renderer/CPURenderer.h>
#include <renderer/EditModeRenderer.h>

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager()
{
	cam = new Camera();
	cam->translate_global(-0.1f, 1, -2);
	cam->rotate(-20, 0, 0);
	escena.set_camera(cam);

	auto meshes = MeshImporter::import_from_file("C:\\Users\\juana\\Desktop\\test3.obj");
	std::vector<Intersectable*> intersectables;
	for(Mesh* m : meshes) {
		intersectables.push_back(dynamic_cast<Intersectable*>(m));
	}
	escena.set_intersectables(intersectables);

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
	escena.add_emitter(l4);
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

void RenderManager::render(int width, int height, int n_samples)
{
	Camera* render_cam = cam->get_copy();
	render_cam->set_width_and_height(width, height);

	CPURenderer::get_renderer()->render(escena, render_cam, n_samples);

	delete render_cam;
}

Image* RenderManager::get_rendered_image()
{
	return CPURenderer::get_renderer()->get_rendered_image();
}

bool RenderManager::is_render_finished()
{
	return CPURenderer::get_renderer()->is_render_finished();
}

void RenderManager::finish_render()
{
	CPURenderer::get_renderer()->set_render_finished(true);
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
			Vector3 color = img->get_pixel_color(i, j) * 255;
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

void RenderManager::set_material(RenderManager::MATERIAL_TYPE m) const
{
	switch(m) {
	case RenderManager::MATERIAL_TYPE::LAMBERTIAN:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Lambertian(Vector3(128)));
		break;
	case RenderManager::MATERIAL_TYPE::BLINNPHONG:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new BlinnPhong());
		break;
	case RenderManager::MATERIAL_TYPE::METAL:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Metal(Vector3(200)));
		break;
	case RenderManager::MATERIAL_TYPE::DIELECTRIC:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Dielectric(Vector3(255), 1.5f));
		break;
	case RenderManager::MATERIAL_TYPE::EMISSIVE:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Emissive(Vector3(255), 5));
		break;
	case RenderManager::MATERIAL_TYPE::REFRACTIVE_LAMBERTIAN:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new RefractiveLambertian(Vector3(128)));
		break;
	}
}

Entity* RenderManager::get_selection(int x, int y)
{
	BVH hierarchy(escena.get_intersectables());
	return cam->get_object(x, y, hierarchy);
}
