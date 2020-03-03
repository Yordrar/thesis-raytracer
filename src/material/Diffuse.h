#pragma once

#include <material/Material.h>

class Diffuse : public Material
{
public:
	~Diffuse() override;

	Ray scatter(Ray ray) override;
};

