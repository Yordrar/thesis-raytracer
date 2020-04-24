#include "CPURenderer.h"

#include <chrono>
#include <iostream>
#include <omp.h>

CPURenderer* CPURenderer::instance = nullptr;

CPURenderer::CPURenderer()
{

}

CPURenderer::~CPURenderer()
{

}

CPURenderer* CPURenderer::get_renderer()
{
	if(!instance) {
		instance = new CPURenderer;
	}
	return instance;
}

void CPURenderer::render(const Scene& scene, Camera* camera, int n_samples)
{
	render_finished = false;

	auto intersectables = scene.get_intersectables();

	int width = camera->get_width();
	int height = camera->get_height();

	if(render_img) delete render_img;
	render_img = new Image(width, height);

	BVH hierarchy(intersectables);
	auto emitters = scene.get_emitters();

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for collapse(2) schedule(dynamic)
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			if(render_finished) continue;
			Vector3 color;
			for(int n = 0; n < n_samples; n++) {
				color += camera->get_color(float(i+Math::Randf()), float(j+Math::Randf()), hierarchy, emitters);
			}
			color /= n_samples;
			color = Vector3(sqrtf(color.get_x()), sqrtf(color.get_y()), sqrtf(color.get_z()));
			render_img->set_pixel_color(i, j, Vector3(color.get_x(), color.get_y(), color.get_z()));
		}
	}
	if(!render_finished) {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "Scene rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms ";
		std::cout << "(" << omp_get_max_threads() << " threads)" << std::endl;
		render_finished = true;
	}
}

