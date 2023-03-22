#include "Vector2.h"
#include "Constants.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

Vector2::Vector2() 
	:x(0), y(0)
{

}

Vector2::Vector2(float x, float y) 
	: x(x), y(y)
{ 

}

float Vector2::Magnitude()
{
	return sqrtf(x * x + y * y);
}

float Vector2::SqrMagnitude()
{
	return (x * x + y * y);
}

Vector2 Vector2::Normal()
{
	Vector2 vec;
	float mag = this->Magnitude();
	
	if (mag == 0)
		return Vector2(0,0);

	vec.x = this->x / mag;
	vec.y = this->y / mag;
	
	return vec;
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return (a - b).Magnitude();
}

float Vector2::AngleRadians(Vector2 a, Vector2 b)
{
	return acosf(Dot(a, b) / (a.Magnitude() * b.Magnitude()));
}

float Vector2::AngleDegrees(Vector2 a, Vector2 b)
{
	return AngleRadians(a, b) * (180.0f / PI);
}

float Vector2::DotClamp(Vector2 a, Vector2 b)
{
	return cos(AngleRadians(a, b));
}

float Vector2::Dot(Vector2 a, Vector2 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

//Default float precision of 6 "(0.000000, 0.000000)";
std::string Vector2::ToString()
{
	std::stringstream xStream;
	std::stringstream yStream;
	xStream << std::fixed << std::setprecision(6) << x;
	yStream << std::fixed << std::setprecision(6) << y;

	return "(" + xStream.str() + ", " + yStream.str() + ")";
}

std::string Vector2::ToString(int float_precision)
{
	std::stringstream xStream;
	std::stringstream yStream;
	xStream << std::fixed << std::setprecision(float_precision) << x;
	yStream << std::fixed << std::setprecision(float_precision) << y;

	return "(" + xStream.str() + ", " + yStream.str() + ")";
}

bool Vector2::operator==(Vector2 rh)
{
	bool xCompare = abs(this->x - rh.x) <= FLOAT_COMPARISION_PRECISION;
	bool yCompare = abs(this->y - rh.y) <= FLOAT_COMPARISION_PRECISION;

	return xCompare && yCompare;
}

void Vector2::operator=(Vector2 rh)
{
	this->x = rh.x;
	this->y = rh.y;
}

Vector2 Vector2::operator+(Vector2 rh)
{
	Vector2 newVec;
	newVec.x = this->x + rh.x;
	newVec.y = this->y + rh.y;
	return newVec;
}

Vector2 Vector2::operator-(Vector2 rh)
{
	Vector2 newVec;
	newVec.x = this->x - rh.x;
	newVec.y = this->y - rh.y;
	return newVec;
}

void Vector2::operator+=(Vector2 rh)
{
	this->x += rh.x;
	this->y += rh.y;
}

void Vector2::operator-=(Vector2 rh)
{
	this->x -= rh.x;
	this->y -= rh.y;
}

Vector2 Vector2::operator*(float num)
{
	Vector2 vec;
	vec.x = this->x * num;
	vec.y = this->y * num;
	return vec;
}

Vector2 Vector2::operator/(float num)
{
	Vector2 vec;
	vec.x = this->x / num;
	vec.y = this->y / num;
	return vec;
}
