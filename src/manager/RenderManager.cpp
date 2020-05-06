#include "OptionsManager.h"
#include "RenderManager.h"
#include "SceneManager.h"

#include <math/Vector3.h>

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
	auto* cam = SceneManager::get_manager()->get_camera();
	cam->set_width_and_height(width, height);

	return EditModeRenderer::render(SceneManager::get_manager()->get_scene(), cam, entity_selected);
}

void RenderManager::render(int width, int height, int n_samples)
{
	Camera* render_cam = SceneManager::get_manager()->get_camera()->get_copy();
	render_cam->set_width_and_height(width, height);
	render_cam->set_aperture(OptionsManager::get_manager()->getCamera_aperture());
	render_cam->set_focus_dist(OptionsManager::get_manager()->getCamera_focus_distance());

	CPURenderer::get_renderer()->render(SceneManager::get_manager()->get_scene(), render_cam, n_samples);

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

Image* RenderManager::get_texture_map() const
{
	Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s)
		return s->get_material()->get_texture_map();
	else
		return nullptr;
}

Image* RenderManager::get_normal_map() const
{
	Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s)
		return s->get_material()->get_normal_map();
	else
		return nullptr;
}

void RenderManager::set_material(RenderManager::MATERIAL_TYPE m) const
{
	switch(m) {
	case RenderManager::MATERIAL_TYPE::LAMBERTIAN:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Lambertian);
		break;
	case RenderManager::MATERIAL_TYPE::BLINNPHONG:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new BlinnPhong);
		break;
	case RenderManager::MATERIAL_TYPE::METAL:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Metal);
		break;
	case RenderManager::MATERIAL_TYPE::DIELECTRIC:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Dielectric);
		break;
	case RenderManager::MATERIAL_TYPE::EMISSIVE:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new Emissive);
		break;
	case RenderManager::MATERIAL_TYPE::REFRACTIVE_LAMBERTIAN:
		dynamic_cast<Scatterer*>(entity_selected)->set_material(new RefractiveLambertian);
		break;
	default:
		break;
	}
}

RenderManager::MATERIAL_TYPE RenderManager::get_material() const
{
	Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s) {
		Material* m = s->get_material();
		if(dynamic_cast<Lambertian*>(m)) {
			return MATERIAL_TYPE::LAMBERTIAN;
		}
		else if(dynamic_cast<BlinnPhong*>(m)) {
			return MATERIAL_TYPE::BLINNPHONG;
		}
		else if(dynamic_cast<Metal*>(m)) {
			return MATERIAL_TYPE::METAL;
		}
		else if(dynamic_cast<Dielectric*>(m)) {
			return MATERIAL_TYPE::DIELECTRIC;
		}
		else if(dynamic_cast<Emissive*>(m)) {
			return MATERIAL_TYPE::EMISSIVE;
		}
		else if(dynamic_cast<RefractiveLambertian*>(m)) {
			return MATERIAL_TYPE::REFRACTIVE_LAMBERTIAN;
		}
	}
	return MATERIAL_TYPE::NONE;
}

void RenderManager::make_selection(int x, int y)
{
	BVH hierarchy(SceneManager::get_manager()->get_scene()->get_intersectables());
	entity_selected = SceneManager::get_manager()->get_camera()->get_object(x, y, hierarchy);
}

bool RenderManager::set_position_entity_selected(float x, float y, float z)
{
	Vector3 old_pos = entity_selected->get_position();
	Vector3 new_position(x, y, z);
	Vector3 delta = new_position - old_pos;
	if(Math::Float_Eq(delta.get_squared_magnitude(), 0.0f))
		return false;
	entity_selected->translate_global(delta.get_x(), delta.get_y(), delta.get_z());
	return true;
}

bool RenderManager::set_orientation_entity_selected(float euler_x, float euler_y, float euler_z)
{
	Vector3 old_rot = entity_selected->get_rotation_euler();
	Vector3 new_orientation(euler_x, euler_y, euler_z);
	Vector3 delta = new_orientation - old_rot;
	if(Math::Float_Eq(delta.get_squared_magnitude(), 0.0f))
		return false;
	entity_selected->rotate_global(delta.get_x(), delta.get_y(), delta.get_z());
	return true;
}

float RenderManager::get_intensity() const
{
	Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s) {
		Emissive* em = dynamic_cast<Emissive*>(s->get_material());
		if(em)
			return em->get_intensity();
		else
			return 0;
	}
	else
		return 0;
}

void RenderManager::set_intensity(float value)
{
	Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s) {
		Emissive* em = dynamic_cast<Emissive*>(s->get_material());
		if(em)
			return em->set_intensity(value);
	}
}

float RenderManager::get_refraction_index() const
{Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s) {
		Dielectric* d = dynamic_cast<Dielectric*>(s->get_material());
		if(d)
			return d->get_refraction_index();
		else
			return 0;
	}
	else
		return 0;
}

void RenderManager::set_refraction_index(float value)
{
	Scatterer* s = dynamic_cast<Scatterer*>(entity_selected);
	if(s) {
		Dielectric* d = dynamic_cast<Dielectric*>(s->get_material());
		if(d)
			return d->set_refraction_index(value);
	}
}
