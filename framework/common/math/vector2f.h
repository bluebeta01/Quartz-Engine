#pragma once

struct Vector2f
{
	Vector2f() {}
	Vector2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float x = 0;
	float y = 0;
};
