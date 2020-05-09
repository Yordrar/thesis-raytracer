#pragma once

#include <string>

#include <math/Vector3.h>

class Image
{
public:
	Image(int width, int height);
	Image(std::string file_path);
	~Image();

	Vector3 get_pixel_color(int x, int y);
	Vector3 get_pixel_color_bilinear_interp(float x, float y);
	void set_pixel_color(int x, int y, const Vector3& color);

	inline int get_width() {return width;}
	inline int get_height() {return height;}

	Image* get_copy() const;

	Vector3 get_mean() const;

	void fill(Vector3 color);

private:
	Vector3** buffer;
	int width, height;
};

