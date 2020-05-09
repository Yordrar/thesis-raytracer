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
										file_data[3*width*j + 3*i+2]) / 255;
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
	x = Math::Clampf(x, 0, width-1);
	y = Math::Clampf(y, 0, height-1);
	return buffer[y][x];
}

Vector3 Image::get_pixel_color_bilinear_interp(float x, float y)
{
	x = Math::Clampf(x, 0, width-1);
	y = Math::Clampf(y, 0, height-1);

	Vector3 q11(floor(x), ceil(y), 0);
	Vector3 q12(floor(x), floor(y), 0);
	Vector3 q21(ceil(x), ceil(y), 0);
	Vector3 q22(ceil(x), floor(y), 0);

	Vector3 fxy1 = (ceil(x) - x) / (ceil(x) - floor(x)) * buffer[static_cast<int>(q11.get_y())][static_cast<int>(q11.get_x())];
	fxy1 += (x  - floor(x)) / (ceil(x) - floor(x)) * buffer[static_cast<int>(q21.get_y())][static_cast<int>(q21.get_x())];

	Vector3 fxy2 = (ceil(x) - x) / (ceil(x) - floor(x)) * buffer[static_cast<int>(q12.get_y())][static_cast<int>(q12.get_x())];
	fxy2 += (x  - floor(x)) / (ceil(x) - floor(x)) * buffer[static_cast<int>(q22.get_y())][static_cast<int>(q22.get_x())];


	Vector3 fxy = (floor(y) - y) / (floor(y) - ceil(y)) * fxy1;
	fxy += (y  - ceil(y)) / (floor(y) - ceil(y)) * fxy2;

	return fxy;
}

void Image::set_pixel_color(int x, int y, const Vector3& color)
{
	if(x < width && x >= 0 && y < height && y >= 0) {
		buffer[y][x] = color;
	}
}

Image* Image::get_copy() const
{
	Image* copy = new Image(width, height);
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			copy->set_pixel_color(i, j, buffer[j][i]);
		}
	}
	return copy;
}

Vector3 Image::get_mean() const
{
	Vector3 color;
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			color += buffer[j][i];
		}
	}
	return color/(width*height);
}

void Image::fill(Vector3 color)
{
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			buffer[j][i] = color;
		}
	}
}
