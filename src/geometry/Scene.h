#pragma once

#include <initializer_list>
#include <vector>

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Emitter.h>
#include <geometry/Camera.h>

#include <image/Image.h>

#include <geometry/mesh/Mesh.h>

class Scene
{
public:
	Scene();
	Scene(const std::initializer_list<Intersectable*>& intersectables);

	inline std::vector<Intersectable*> get_intersectables() const {return intersectables;}
	inline void set_intersectables(std::vector<Intersectable*> value) {
		if(intersectables.size() > 0) {
			for(Intersectable* i : intersectables) {
				if(dynamic_cast<Mesh*>(i)) {
					delete dynamic_cast<Mesh*>(i);
					continue;
				}
				if(dynamic_cast<Sphere*>(i)) {
					delete dynamic_cast<Sphere*>(i);
					continue;
				}
			}
		}
		intersectables = value;
	}
	inline std::vector<Emitter*> get_emitters() const {return emitters;}

	inline void add_intersectable(Intersectable* e) {intersectables.push_back(e);}
	Intersectable* get_intersectable(int index) const;
	void remove_intersectable(Intersectable* e);

	inline void add_emitter(Emitter* e) {emitters.push_back(e);}

private:
	std::vector<Intersectable*> intersectables;
	std::vector<Emitter*> emitters;
};

