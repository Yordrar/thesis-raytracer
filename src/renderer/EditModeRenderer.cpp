#include "EditModeRenderer.h"

#include <chrono>
#include <iostream>
#include <omp.h>

#include <geometry/BVH/RandomAxis.h>

Image EditModeRenderer::render(const Scene* scene, Camera* camera, Entity* entity_selected)
{
	auto intersectables = scene->get_intersectables();

	int width = camera->get_width();
	int height = camera->get_height();

	Image framebuffer(width, height);

	if(intersectables.size() <= 0) {
		framebuffer.fill(Vector3(sqrtf(0.1f)));
		return framebuffer;
	}

	BVHBuildStrategy* strategy = new RandomAxis();
	BVH hierarchy(intersectables, strategy);
	delete dynamic_cast<RandomAxis*>(strategy);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for collapse(2) schedule(dynamic) shared(framebuffer)
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			Vector3 color = camera->get_color_preview(i, j, hierarchy, entity_selected);
			color = Vector3(sqrtf(color.get_x()), sqrtf(color.get_y()), sqrtf(color.get_z()));
			framebuffer.set_pixel_color(i, j, Vector3(color.get_x(), color.get_y(), color.get_z()));
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Preview rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms ";
	std::cout << "(" << omp_get_max_threads() << " threads)" << std::endl;

	return framebuffer;
}
