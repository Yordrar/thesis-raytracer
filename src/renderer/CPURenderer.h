#pragma once

#include <image/Framebuffer.h>

#include <geometry/Scene.h>

class CPURenderer
{
public:
	CPURenderer() = delete;
	~CPURenderer() = delete;

	static Framebuffer render(const Scene& scene, int n_samples);
};

