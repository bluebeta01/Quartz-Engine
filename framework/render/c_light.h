#pragma once
#include "framework/pch.h"

class Light
{
public:
	float brightness = 1.0f;
	glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 position = glm::vec3(0, 0, 0);
	Light();
	virtual ~Light() {}
};

