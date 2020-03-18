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
	Scene(Camera* c);

	inline std::vector<Intersectable*> get_entities() const {return intersectables;}

	inline Camera* get_camera() const {return camera;}
	inline void set_camera(Camera* cam) {camera = cam;}

	inline void add_intersectable(Intersectable* e) {intersectables.push_back(e);}
	Intersectable* get_intersectable(int index) const;
	void remove_intersectable(Intersectable* e);
	inline std::vector<Intersectable*> get_intersectables() const {return intersectables;}

private:
	std::vector<Intersectable*> intersectables;
	Camera* camera;
};

