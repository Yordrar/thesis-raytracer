#pragma once

#include <initializer_list>
#include <vector>

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Camera.h>

#include <image/Framebuffer.h>

class Scene
{
public:
	Scene();
	Scene(std::initializer_list<Intersectable*> intersectables);
	Scene(Camera c);

	inline std::vector<Intersectable*> get_entities() const {return intersectables;}

	inline void set_camera(Camera cam) {camera = cam;}

	inline void add_intersectable(Intersectable* e) {intersectables.push_back(e);}
	Intersectable* get_intersectable(int index) const;
	void remove_intersectable(Intersectable* e);

	Framebuffer render(int n_samples) const;

private:
	std::vector<Intersectable*> intersectables;
	Camera camera;
};

