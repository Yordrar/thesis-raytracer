#include "Scene.h"

#include <fstream>
#include <iostream>
#include <chrono>

#include <math/Vector3.h>

#include <QImage>

Scene::Scene()
{

}

Scene::Scene(std::initializer_list<Intersectable*> entitites)
{
	for(auto e : entitites) {
		this->intersectables.push_back(e);
	}
}

Scene::Scene(Camera c)
{
	camera = c;
}

Intersectable* Scene::get_intersectable(int index) const
{
	if(index < 0 || index >= static_cast<int>(intersectables.size())) {
		return nullptr;
	}
	auto it = intersectables.begin();
	std::advance(it, index);
	return *it;
}

void Scene::remove_intersectable(Intersectable* e)
{
	auto it = intersectables.begin();
	while(it != intersectables.end()) {
		if(*it == e) {
			intersectables.erase(it);
			return;
		}
		it++;
	}
}

QPixmap Scene::render() const
{
	int width = camera.get_width();
	int height = camera.get_height();
	int n_samples = 50;

	QImage framebuffer(width, height, QImage::Format_RGB888);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#pragma omp parallel for collapse(2) schedule(dynamic) shared(framebuffer)
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			Vector3 color;
			for(int n = 0; n < n_samples; n++) {
				color += camera.get_color(float(i+Math::Randf()), float(j+Math::Randf()), intersectables);
			}
			color /= n_samples;
			color = Vector3(sqrtf(color.get_x()), sqrtf(color.get_y()), sqrtf(color.get_z()));
			framebuffer.setPixelColor(i, j, QColor(color.get_x()*255.99f,color.get_y()*255.99f,color.get_z()*255.99f));
		}
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Scene rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

	return QPixmap::fromImage(framebuffer);
}
