#pragma once
#include <string>

struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);

	float Magnitude();
	float SqrMagnitude();
	Vector2 Normal();	
	static float Distance(Vector2 a, Vector2 b);
	static float AngleRadians(Vector2 a, Vector2 b);
	static float AngleDegrees(Vector2 a, Vector2 b);
	static float DotClamp(Vector2 a, Vector2 b);
	static float Dot(Vector2 a, Vector2 b);

	std::string ToString();
	std::string ToString(int float_precision);
	
	bool operator==(Vector2 rh);
	void operator=(Vector2 rh);
	Vector2 operator+(Vector2 rh);
	Vector2 operator-(Vector2 rh);
	void operator+=(Vector2 rh);
	void operator-=(Vector2 rh);
	Vector2 operator*(float num);
	Vector2 operator/(float num);
};