#include "Entity.h"

Entity::Entity(Vector3 position, Quaternion rotation)
	: position(position),
	  rotation(rotation)
{

}

Entity::~Entity()
{

}

Vector3 orthogonal(Vector3 v)
{
	float x = fabsf(v.get_x());
	float y = fabsf(v.get_y());
	float z = fabsf(v.get_z());

	Vector3 other = x < y ? (x < z ? Vector3(1, 0, 0) : Vector3(0, 0, 1)) : (y < z ? Vector3(0, 1, 0) : Vector3(0, 0, 1));
	return v.cross(other);
}

Quaternion Entity::get_rotation() const
{
	Vector3 u(0, 0, 1);
	Vector3 v = rotation.get_imaginary();
	float k_cos_theta = u.dot(v);
	float k = sqrtf(u.get_squared_magnitude() * v.get_squared_magnitude());

	if (k_cos_theta / k == -1) {
		// 180 degree rotation around any orthogonal vector
		return Quaternion(0, orthogonal(u).unit());
	}

	return Quaternion(k_cos_theta + k, u.cross(v)).unit();
}

Vector3 Entity::get_rotation_euler() const
{
	Quaternion rotation = get_rotation();
	float heading, attitude, bank;
	double sqw = rotation.get_w()*rotation.get_w();
	double sqx = rotation.get_x()*rotation.get_x();
	double sqy = rotation.get_y()*rotation.get_y();
	double sqz = rotation.get_z()*rotation.get_z();

	// If normalised this is one, otherwise is correction factor
	// Saves normalising the quaternion
	double unit = sqx + sqy + sqz + sqw;
	double test = rotation.get_x()*rotation.get_y() + rotation.get_z()*rotation.get_w();

	// Special cases
	if (test > 0.499*unit) { // singularity at north pole
		heading = 2 * atan2(rotation.get_x(),rotation.get_w());
		attitude = Math::PI/2;
		bank = 0;
		return Vector3(heading, attitude, bank);
	}
	if (test < -0.499*unit) { // singularity at south pole
		heading = -2 * atan2(rotation.get_x(),rotation.get_w());
		attitude = -Math::PI/2;
		bank = 0;
		return Vector3(heading, attitude, bank);
	}

	heading = atan2(2*rotation.get_y()*rotation.get_w()-2*rotation.get_x()*rotation.get_z() , sqx - sqy - sqz + sqw);
	attitude = asin(2*test/unit);
	bank = atan2(2*rotation.get_x()*rotation.get_w()-2*rotation.get_y()*rotation.get_z() , -sqx + sqy - sqz + sqw);
	return Vector3(Math::Rad2Deg(bank), Math::Rad2Deg(heading), Math::Rad2Deg(attitude));
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
	rotation = q.apply(rotation);
}

void Entity::rotate_local(float euler_x, float euler_y, float euler_z)
{

}

void Entity::rotate(const Quaternion& rotation)
{
	this->rotation = rotation.apply(this->rotation);
}
