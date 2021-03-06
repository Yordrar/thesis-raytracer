#pragma once

#include <image/Image.h>

#include <geometry/Scene.h>

#include <atomic>

class CPURenderer
{
public:
	~CPURenderer();
	static CPURenderer* get_renderer();

	void render(const Scene* scene, Camera* camera, int n_samples);
	Image* get_rendered_image() {return render_img;}
	bool is_render_finished() {return render_finished;}
	void set_render_finished(bool value) {render_finished = value;}
	inline int get_samples_rendered() const {return samples_rendered.load();}

private:
	CPURenderer();
	static CPURenderer* instance;

	Image* render_img = nullptr;
	bool render_finished = false;
	std::atomic<int> samples_rendered;
};

