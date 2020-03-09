#pragma once

#include <math/Vector3.h>

class Framebuffer
{
public:
	Framebuffer(int width, int height);
	~Framebuffer();

	Vector3 get_pixel_color(int x, int y);
	void set_pixel_color(int x, int y, Vector3 color);

	inline int get_width() {return width;}
	inline int get_height() {return height;}

private:
	Vector3** buffer;
	int width, height;
};

