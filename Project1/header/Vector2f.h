#pragma once
#include <iostream>
#include <string>
class Vector2f
{
public:
	Vector2f();
	Vector2f(float _x, float _y);
	~Vector2f();

	//Operator overloading
	void operator=(const Vector2f &v); //Assignment operator
	bool operator==(const Vector2f &v);//Equals operator
	bool operator!=(const Vector2f &v);//Equals operator
	Vector2f operator/(const float val); //Division operator
	Vector2f operator+(const Vector2f &v); //Plus operator
	void operator+=(const Vector2f &v); //Plus equals operator
	void operator-=(const Vector2f &v); //Minus equals operator
	void operator/=(const Vector2f &v); //Division equals operator
	void operator*=(float val); //Multiplication equals operator
	Vector2f operator-(const Vector2f &v); //Minus operator
	Vector2f operator*(float val); //Multiplication operator
	friend std::ostream & operator << (std::ostream &out, const Vector2f &v);

	//Vector operations
	float distance(Vector2f& v1, Vector2f& v2);
	float magnitude();
	float sqrdMagnitude();
	float dotProduct(Vector2f v);
	Vector2f normalise();
	void zeroVector(); //Reset vector to 0,0

	//Public variables
	float x, y;
};

