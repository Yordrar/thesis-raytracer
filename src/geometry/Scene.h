#pragma once

#include <initializer_list>
#include <list>

#include <geometry/Entity.h>
#include <geometry/Intersectable.h>
#include <geometry/Camera.h>

#include <QPixmap>

class Scene
{
public:
	Scene();
	Scene(std::initializer_list<Intersectable*> intersectables);
	Scene(Camera c);

	inline std::list<Intersectable*> get_entities() const {return intersectables;}

	inline void set_camera(Camera cam) {camera = cam;}

	inline void add_intersectable(Intersectable* e) { intersectables.push_front(e); }
	Intersectable* get_intersectable(int index) const;
	void remove_intersectable(Intersectable* e);

	QPixmap render() const;

private:
	std::list<Intersectable*> intersectables;
	Camera camera;
};

