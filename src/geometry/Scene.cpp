#include "Scene.h"

#include <fstream>
#include <iostream>
#include <chrono>

Scene::Scene()
{

}

Scene::Scene(std::initializer_list<Entity*> entitites)
{
	for(auto e : entitites) {
		this->entities.push_back(e);
	}
}

Scene::Scene(Camera c)
{
	camera = c;
}

Entity* Scene::get_entity(int index) const
{
	if(index < 0 || index >= static_cast<int>(entities.size())) {
		return nullptr;
	}
	auto it = entities.begin();
	std::advance(it, index);
	return *it;
}

void Scene::remove_entity(Entity* e)
{
	auto it = entities.begin();
	while(it != entities.end()) {
		if(*it == e) {
			entities.erase(it);
			return;
		}
		it++;
	}
}

QPixmap Scene::render() const
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	float width = camera.get_width();
	float height = camera.get_height();
	int n_samples = 50;
	std::fstream image("image.ppm", std::fstream::out | std::fstream::trunc);
	image << "P3\n" << width << " " << height << "\n255\n";
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			Vector3 color;
			for(int n = 0; n < n_samples; n++) {
				color += camera.get_color(float(i+Math::Randf()), float(j+Math::Randf()), entities);
			}
			color /= n_samples;
			color = Vector3(sqrtf(color.get_x()), sqrtf(color.get_y()), sqrtf(color.get_z()));
			image << int(color.get_x()*255.99f) << " " << int(color.get_y()*255.99f) << " " << int(color.get_z()*255.99f) << "\n";
		}
	}
	image.close();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Scene rendered in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
	return QPixmap("image.ppm");
}
