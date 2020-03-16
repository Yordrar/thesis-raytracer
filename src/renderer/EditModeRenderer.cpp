#include "EditModeRenderer.h"

#include <chrono>
#include <iostream>
#include <omp.h>

Framebuffer EditModeRenderer::render(const Scene& scene)
{
	Camera camera = scene.get_camera();
	auto intersectables = scene.get_intersectables();

	int width = camera.get_width();
	int height = camera.get_height();

	Framebuffer framebuffer(width, height);

	BVH hierarchy(intersectables);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for collapse(2) schedule(dynamic) shared(framebuffer)
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			Vector3 color = camera.get_color_edit_mode(i, j, hierarchy);
			color = Vector3(sqrtf(color.get_x()), sqrtf(color.get_y()), sqrtf(color.get_z()));
			framebuffer.set_pixel_color(i, j, Vector3(color.get_x()*255.99f,
													color.get_y()*255.99f,
													color.get_z()*255.99f));
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Scene rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms ";
	std::cout << "(" << omp_get_max_threads() << " threads)" << std::endl;

	return framebuffer;
}
