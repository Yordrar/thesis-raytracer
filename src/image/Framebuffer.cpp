#include "Framebuffer.h"


Framebuffer::Framebuffer(int width, int height)
	: width(width),
	  height(height)
{
	buffer = new Vector3*[height];
	for(int i = 0; i < height; i++) {
		buffer[i] = new Vector3[width];
	}
}

Framebuffer::~Framebuffer()
{
	for(int i = 0; i < height; i++) {
		delete buffer[i];
	}
	delete buffer;
}

Vector3 Framebuffer::get_pixel_color(int x, int y)
{
	if(x >= width || x < 0 || y >= height || y < 0) {
		return Vector3();
	}
	return buffer[y][x];
}

void Framebuffer::set_pixel_color(int x, int y, const Vector3& color)
{
	if(x < width && x >= 0 && y < height && y >= 0) {
		buffer[y][x] = color;
	}
}
