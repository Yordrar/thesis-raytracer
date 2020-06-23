#pragma once

#include <math/Vector3.h>

#include <image/Image.h>

class OptionsManager
{
public:
	~OptionsManager();
	static OptionsManager* get_manager();

    enum class IMAGE_BASED_LIGHTNING_TYPE {
		GRADIENT,
		ENVIRONMENT_MAP,
	};

    IMAGE_BASED_LIGHTNING_TYPE get_light_type() const;
    void set_light_type(const IMAGE_BASED_LIGHTNING_TYPE& value);

	Vector3 get_gradient_start_color() const;
	void set_gradient_start_color(const Vector3& value);

	Vector3 get_gradient_end_color() const;
	void set_gradient_end_color(const Vector3& value);

	Image* get_environment_map() const;
	void set_environment_map(std::string filename);

	float getCamera_move_velocity() const;
	void setCamera_move_velocity(float value);

	float getCamera_zoom_velocity() const;
	void setCamera_zoom_velocity(float value);

	float getCamera_aperture() const;
	void setCamera_aperture(float value);

	float getCamera_focus_distance() const;
	void setCamera_focus_distance(float value);

private:
	OptionsManager();
	static OptionsManager* instance;

    IMAGE_BASED_LIGHTNING_TYPE light_type = IMAGE_BASED_LIGHTNING_TYPE::GRADIENT;
	Vector3 gradient_start_color = Vector3(128, 179, 255)/255.0f;
	Vector3 gradient_end_color = Vector3(255, 255, 255)/255.0f;
	Image* environment_map = nullptr;


    float camera_move_velocity = 0.1f, camera_zoom_velocity = 2, camera_aperture = 0, camera_focus_distance = 1;
};

