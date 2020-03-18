#include "Entity.h"

Entity::Entity(Vector3 position, Quaternion rotation)
	: position(position),
	  orientation(rotation)
{

}

void Entity::translate(float delta_x, float delta_y, float delta_z)
{
	position += Vector3(delta_x, delta_y, delta_z);
}

void Entity::translate(Vector3 delta)
{
	position += delta;
}

void Entity::rotate(float euler_x, float euler_y, float euler_z)
{
	Quaternion qx = Quaternion::create_rotation(euler_x, Vector3(1, 0, 0));
	Quaternion qy = Quaternion::create_rotation(euler_y, Vector3(0, 1, 0));
	Quaternion qz = Quaternion::create_rotation(euler_z, Vector3(0, 0, 1));
	Quaternion q = qz*(qy*qx).unit();
	orientation = q.apply(orientation);
}

void Entity::rotate(Quaternion rotation)
{
	orientation = rotation.apply(orientation);
}

Entity::~Entity()
{

}
