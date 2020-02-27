#include <gtest/gtest.h>

#include <geometry/Scene.h>
#include <geometry/Sphere.h>
#include <math/Vector3.h>

#include <list>

TEST(Scene, Constructors) {
	Scene s1;
	EXPECT_EQ(s1.get_entities(), std::list<Entity*>());

	Sphere sph1(Vector3(), 3);
	Scene s2{&sph1, &sph1, &sph1, &sph1};
	for(auto e : s2.get_entities()) {
		EXPECT_EQ(e->get_position(), Vector3());
	}
}

TEST(Scene, AddEntity) {
	Sphere sph1(Vector3(), 3);
	Scene s1{&sph1, &sph1, &sph1, &sph1};
	EXPECT_EQ(s1.get_entities().size(), 4);

	s1.add_entity(&sph1);
	EXPECT_EQ(s1.get_entities().size(), 5);
}

TEST(Scene, GetEntity) {
	Sphere sph1(Vector3(), 3);
	Scene s1{&sph1, &sph1, &sph1, &sph1};
	EXPECT_EQ(s1.get_entity(0), &sph1);
	EXPECT_EQ(s1.get_entity(5), nullptr);
	EXPECT_EQ(s1.get_entity(-1), nullptr);
}

TEST(Scene, RemoveEntity) {
	Sphere sph1(Vector3(), 3);
	Scene s1{&sph1, &sph1, &sph1, &sph1};
	EXPECT_EQ(s1.get_entities().size(), 4);

	s1.remove_entity(&sph1);
	EXPECT_EQ(s1.get_entities().size(), 3);
}
