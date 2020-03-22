#pragma once

#include <image/Image.h>

#include <geometry/Scene.h>

class CPURenderer
{
public:
	CPURenderer() = delete;
	~CPURenderer() = delete;

	static Image render(const Scene& scene, int n_samples);
};

