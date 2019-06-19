#pragma once
#include "c_light.h"
class PointLight :
	public Light
{
public:
	float radius = 20;
	PointLight();
	~PointLight();
};

