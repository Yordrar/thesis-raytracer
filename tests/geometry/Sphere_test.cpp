#include <gtest/gtest.h>

#include <geometry/Sphere.h>
#include <math/Vector3.h>
#include <math/Math.h>
#include <math/Ray.h>
#include <material/Lambertian.h>
#include <material/Metal.h>

TEST(Sphere, Intersect) {
	Ray r(Vector3(0, 0, 0), Vector3(0, 0, 1));
	Sphere s(Vector3(0, 0, 5), 2);

	EXPECT_TRUE(Math::Float_Eq(s.get_intersection(r).get_t(), 3.0f));

	s.set_position(Vector3(5, 0, 0));
	EXPECT_FALSE(s.get_intersection(r).is_hit());
}

TEST(Sphere, GetNormal) {
	Sphere s(Vector3(0, 0, 0), 1);
	Hit h = s.get_intersection(Ray(Vector3(0, 0, 3), Vector3(0, 0, -1)));
	EXPECT_TRUE(h.get_normal() == Vector3(0, 0, 1));

	h = s.get_intersection(Ray(Vector3(3, 0, 0), Vector3(-1, 0, 0)));
	EXPECT_TRUE(h.get_normal() == Vector3(1, 0, 0));

	h = s.get_intersection(Ray(Vector3(0, 3, 0), Vector3(0, -1, 0)));
	EXPECT_TRUE(h.get_normal() == Vector3(0, 1, 0));
}

TEST(Sphere, ChangeMaterial) {
	Sphere s(Vector3(0, 0, 0), 1);
	Material* m1 = new Lambertian;
	Material* m2 = new Metal;
	s.set_material(m1);
	s.set_material(m2);
	EXPECT_EQ(s.get_material(), m2);
}
