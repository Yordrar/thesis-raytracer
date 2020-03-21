#pragma once

#include <initializer_list>
#include <vector>

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Emitter.h>
#include <geometry/Camera.h>

#include <image/Framebuffer.h>

class Scene
{
public:
	Scene();
	Scene(const std::initializer_list<Intersectable*>& intersectables);
	Scene(Camera* c);

	inline std::vector<Intersectable*> get_intersectables() const {return intersectables;}
	inline std::vector<Emitter*> get_emitters() const {return emitters;}

	inline Camera* get_camera() const {return camera;}
	inline void set_camera(Camera* cam) {camera = cam;}

	inline void add_intersectable(Intersectable* e) {intersectables.push_back(e);}
	Intersectable* get_intersectable(int index) const;
	void remove_intersectable(Intersectable* e);

	inline void add_emitter(Emitter* e) {emitters.push_back(e);}

private:
	std::vector<Intersectable*> intersectables;
	std::vector<Emitter*> emitters;
	Camera* camera;
};

