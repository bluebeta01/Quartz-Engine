#include "framework/pch.h"
#include "c_pointlightcomponent.h"

PointLightComponent::PointLightComponent() : Component()
{

}

PointLightComponent::~PointLightComponent()
{

}

void PointLightComponent::setColor(glm::vec3 color) { this->color = color; }
glm::vec3 PointLightComponent::getColor() { return this->color; }
void PointLightComponent::setDistance(float distance) { this->distance = distance; }
float PointLightComponent::getDistance() { return this->distance; }
void PointLightComponent::setBrightness(float brigtness) { this->brightness = brightness; }
float PointLightComponent::getBrightness() { return this->brightness; }