#include "Scene.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <omp.h>

#include <math/Vector3.h>

Scene::Scene()
{

}

Scene::Scene(std::initializer_list<Intersectable*> entitites)
{
	for(auto e : entitites) {
		this->intersectables.push_back(e);
	}
}

Scene::Scene(Camera* c)
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
