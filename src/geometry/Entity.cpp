#include "Entity.h"

Entity::Entity(Vector3 position, Quaternion rotation)
	: position(position),
	  orientation(rotation)
{

}

Entity::~Entity()
{

}

void Entity::translate_global(float delta_x, float delta_y, float delta_z)
{
	position += Vector3(delta_x, delta_y, delta_z);
}

void Entity::translate_global(const Vector3& delta)
{
	position += delta;
}

void Entity::translate_local(float delta_x, float delta_y, float delta_z)
{

}

void Entity::translate_local(const Vector3& delta)
{

}

void Entity::rotate_global(float euler_x, float euler_y, float euler_z)
{
	Quaternion qx = Quaternion::create_rotation(euler_x, Vector3(1, 0, 0));
	Quaternion qy = Quaternion::create_rotation(euler_y, Vector3(0, 1, 0));
	Quaternion qz = Quaternion::create_rotation(euler_z, Vector3(0, 0, 1));
	Quaternion q = qz*(qy*qx).unit();
	orientation = q.apply(orientation);
}

void Entity::rotate_local(float euler_x, float euler_y, float euler_z)
{

}

void Entity::rotate(const Quaternion& rotation)
{
	orientation = rotation.apply(orientation);
}
