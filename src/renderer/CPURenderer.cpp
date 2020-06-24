#include "CPURenderer.h"

#include <chrono>
#include <iostream>
#include <omp.h>

#include <geometry/BVH/RandomAxis.h>
#include <geometry/BVH/SurfaceAreaHeuristic.h>

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

void CPURenderer::render(const Scene* scene, Camera* camera, int n_samples)
{
	render_finished = false;
	samples_rendered = 0;

	auto intersectables = scene->get_intersectables();

	int width = camera->get_width();
	int height = camera->get_height();

	if(render_img) delete render_img;
	render_img = new Image(width, height);

	if(intersectables.size() <= 0) {
		render_img->fill(Vector3(0));
		render_finished = true;
		return;
	}

	std::chrono::steady_clock::time_point bvh_begin = std::chrono::steady_clock::now();
	BVHBuildStrategy* strategy = new RandomAxis();
	BVH hierarchy(intersectables, strategy);
	delete dynamic_cast<RandomAxis*>(strategy);
	std::chrono::steady_clock::time_point bvh_end = std::chrono::steady_clock::now();
	std::cout << "CPURenderer BVH built with: " << hierarchy.get_num_nodes()
			  << " internal nodes and " << hierarchy.get_num_intersectables()
			  << " leaf nodes" <<
			  " in " << std::chrono::duration_cast<std::chrono::milliseconds>(bvh_end - bvh_begin).count() << " ms " << std::endl;
	auto emitters = scene->get_emitters();

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for collapse(2) schedule(dynamic)
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			if(render_finished) continue;
			Vector3 color;
			for(int n = 0; n < n_samples; n++) {
				color += camera->get_color(float(i+Math::Randf()), float(j+Math::Randf()), hierarchy, emitters, intersectables);
            }
            color /= n_samples;
            color = Vector3(sqrtf(color.get_x()), sqrtf(color.get_y()), sqrtf(color.get_z()));
            render_img->set_pixel_color(i, j, color);
			samples_rendered++;
		}
	}
	if(!render_finished) {
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() > 1000) {
			std::cout << "Scene rendered in ";
			int hours = std::chrono::duration_cast<std::chrono::hours>(end - begin).count();
			std::cout << (hours < 10 ? "0"+std::to_string(hours) : std::to_string(hours)) << ":";
			int minutes = std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() % 60;
			std::cout << (minutes < 10 ? "0"+std::to_string(minutes) : std::to_string(minutes)) << ":";
			int seconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() % 60;
			std::cout << (seconds < 10 ? "0"+std::to_string(seconds) : std::to_string(seconds)) << " ";
		}
		else {
			std::cout << "Scene rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms ";
		}
		std::cout << "(" << omp_get_max_threads() << " threads)" << std::endl;
		render_finished = true;
	}
}

