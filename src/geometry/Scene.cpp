#include "Scene.h"

#include <fstream>

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

inline float random_double() {
	return rand() / (RAND_MAX + 1.0f);
}
QPixmap Scene::render() const
{
	float width = camera.get_width();
	float height = camera.get_height();
	int n_samples = 50;
	std::fstream image("image.ppm", std::fstream::out | std::fstream::trunc);
	image << "P3\n" << width << " " << height << "\n255\n";
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			Vector3 color;
			for(int n = 0; n < n_samples; n++) {
				float min_t = FLT_MAX;
				Entity* intersected_e = nullptr;
				Ray r = camera.get_ray(float(i+random_double()), float(j+random_double()));
				for(auto e : entities) {
					float t = e->get_intersection(r);
					if(t > 0.0f && t < min_t) {
						min_t = t;
						intersected_e = e;
					}
				}
				if(intersected_e) {
					Vector3 normal = intersected_e->get_normal(r.get_point(min_t));
					normal = (normal+Vector3(1.0f, 1.0f, 1.0f))*0.5f;
					color += Vector3(normal.get_x(), normal.get_y(), normal.get_z());
				}
				else {
					Vector3 unit_direction = r.get_direction().unit();
					float t = 0.5f*(unit_direction.get_y() + 1.0f);
					color += (1.0f-t)*Vector3(1.0f, 1.0f, 1.0f) + t*Vector3(0.5f, 0.7f, 1.0f);
				}
			}
			color /= n_samples;
			image << int(color.get_x()*255.99f) << " " << int(color.get_y()*255.99f) << " " << int(color.get_z()*255.99f) << "\n";
		}
	}
	image.close();
	return QPixmap("image.ppm");
}
