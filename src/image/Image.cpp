#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(int width, int height)
	: width(width),
	  height(height)
{
	buffer = new Vector3*[height];
	for(int i = 0; i < height; i++) {
		buffer[i] = new Vector3[width];
	}
}

Image::Image(std::string file_path)
{
	int x, y, n;
	unsigned char* file_data = stbi_load(file_path.c_str(), &x, &y, &n, 3);
	width = x;
	height = y;
	buffer = new Vector3*[height];
	for(int i = 0; i < height; i++) {
		buffer[i] = new Vector3[width];
	}
	if(file_data) {
		for(int j = 0; j < height; j++) {
			for(int i = 0; i < width; i++) {
				buffer[j][i] = Vector3(file_data[3*width*j + 3*i],
										file_data[3*width*j + 3*i+1],
										file_data[3*width*j + 3*i+2]) / 255.99f;
			}
		}
	}
}

Image::~Image()
{
	for(int i = 0; i < height; i++) {
		delete buffer[i];
	}
	delete buffer;
}

Vector3 Image::get_pixel_color(int x, int y)
{
	if(x >= width || x < 0 || y >= height || y < 0) {
		return Vector3();
	}
	return buffer[y][x];
}

void Image::set_pixel_color(int x, int y, const Vector3& color)
{
	if(x < width && x >= 0 && y < height && y >= 0) {
		buffer[y][x] = color;
	}
}
