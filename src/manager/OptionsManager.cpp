#include "OptionsManager.h"

OptionsManager* OptionsManager::instance = nullptr;

OptionsManager::OptionsManager()
{

}

OptionsManager::~OptionsManager()
{

}

OptionsManager* OptionsManager::get_manager()
{
	if(!instance) {
		instance = new OptionsManager;
	}
	return instance;
}

float OptionsManager::getCamera_focus_distance() const
{
	return camera_focus_distance;
}

void OptionsManager::setCamera_focus_distance(float value)
{
	camera_focus_distance = value;
}

float OptionsManager::getCamera_aperture() const
{
	return camera_aperture;
}

void OptionsManager::setCamera_aperture(float value)
{
	camera_aperture = value;
}

float OptionsManager::getCamera_zoom_velocity() const
{
	return camera_zoom_velocity;
}

void OptionsManager::setCamera_zoom_velocity(float value)
{
	camera_zoom_velocity = value;
}

float OptionsManager::getCamera_move_velocity() const
{
	return camera_move_velocity;
}

void OptionsManager::setCamera_move_velocity(float value)
{
	camera_move_velocity = value;
}

Image* OptionsManager::get_environment_map() const
{
	return environment_map;
}

void OptionsManager::set_environment_map(std::string filename)
{
	environment_map = new Image(filename);
}

Vector3 OptionsManager::get_gradient_end_color() const
{
	return gradient_end_color;
}

void OptionsManager::set_gradient_end_color(const Vector3& value)
{
	gradient_end_color = value/255;
}

Vector3 OptionsManager::get_gradient_start_color() const
{
	return gradient_start_color;
}

void OptionsManager::set_gradient_start_color(const Vector3& value)
{
	gradient_start_color = value/255;
}

OptionsManager::IMAGE_BASED_LIGHT_TYPE OptionsManager::get_light_type() const
{
	return light_type;
}

void OptionsManager::set_light_type(const IMAGE_BASED_LIGHT_TYPE& value)
{
	light_type = value;
}
