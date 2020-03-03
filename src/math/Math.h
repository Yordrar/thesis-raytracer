#pragma once

#include <cmath>
#include <cfloat>

class Math
{
public:
	Math() = delete;
	~Math() = delete;

	static constexpr float PI = 3.14159265f;

	inline static bool Float_Eq(float a, float b) {return fabsf(a-b) <= FLT_EPSILON;}
	inline static int Clamp(int value, int min_value, int max_value) {return static_cast<int>(Clampf(value, min_value, max_value));}
	inline static float Clampf(float value, float min_value, float max_value) {return fmaxf(min_value, fminf(value, max_value));}
	inline static float Map(float value, float in_min, float in_max, float out_min, float out_max) {
		return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
	inline static float Randf() {return rand() / (RAND_MAX + 1.0f);}
	inline static float Rad2Deg(float radians) {return radians * (180.0f/PI);}
	inline static float Deg2Rad(float degrees) {return degrees * (PI/180.0f);}
};

