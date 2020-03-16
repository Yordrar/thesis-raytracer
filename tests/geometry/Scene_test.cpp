#include <gtest/gtest.h>

#include <geometry/Scene.h>
#include <geometry/Sphere.h>
#include <math/Vector3.h>

#include <list>

TEST(Scene, Constructors) {
	Scene s1;
	EXPECT_EQ(s1.get_entities().size(), 0);

	Sphere sph1(Vector3(), 3);
	Scene s2{&sph1, &sph1, &sph1, &sph1};
	for(auto e : s2.get_entities()) {
		EXPECT_EQ(dynamic_cast<Entity*>(e)->get_position(), Vector3());
	}
}

TEST(Scene, AddIntersectable) {
	Sphere sph1(Vector3(), 3);
	Scene s1{&sph1, &sph1, &sph1, &sph1};
	EXPECT_EQ(s1.get_entities().size(), 4);

	s1.add_intersectable(&sph1);
	EXPECT_EQ(s1.get_entities().size(), 5);
}

TEST(Scene, GetIntersectable) {
	Sphere sph1(Vector3(), 3);
	Scene s1{&sph1, &sph1, &sph1, &sph1};
	EXPECT_EQ(s1.get_intersectable(0), &sph1);
	EXPECT_EQ(s1.get_intersectable(5), nullptr);
	EXPECT_EQ(s1.get_intersectable(-1), nullptr);
}

TEST(Scene, RemoveIntersectable) {
	Sphere sph1(Vector3(), 3);
	Scene s1{&sph1, &sph1, &sph1, &sph1};
	EXPECT_EQ(s1.get_entities().size(), 4);

	s1.remove_intersectable(&sph1);
	EXPECT_EQ(s1.get_entities().size(), 3);
}
