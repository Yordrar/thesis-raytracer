#pragma once

#include <material/Material.h>

class Metal : public Material
{
public:
	~Metal() override;

	Ray scatter(Ray ray) override;
};

