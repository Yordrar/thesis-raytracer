#pragma once

#include <initializer_list>
#include <list>

#include <geometry/Entity.h>
#include <geometry/Camera.h>

#include <QPixmap>

class Scene
{
public:
	Scene();
	Scene(std::initializer_list<Entity*> entities);
	Scene(Camera c);

	inline std::list<Entity*> get_entities() const {return entities;}

	inline void set_camera(Camera cam) {camera = cam;}

	inline void add_entity(Entity* e) { entities.push_front(e); }
	Entity* get_entity(int index) const;
	void remove_entity(Entity* e);

	QPixmap render() const;

private:
	std::list<Entity*> entities;
	Camera camera;
};

