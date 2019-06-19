#pragma once
#include "c_component.h"
#include "framework/pch.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent();
	~PointLightComponent();

	void setColor(glm::vec3 color);
	glm::vec3 getColor();
	void setDistance(float distance);
	float getDistance();
	void setBrightness(float brigtness);
	float getBrightness();

private:
	glm::vec3 color;
	float distance;
	float brightness;
};