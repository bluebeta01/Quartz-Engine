#include "framework/pch.h"
#include "c_rendercomponent.h"

RenderComponent::RenderComponent() : Component()
{

}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::setModel(Model* model)
{
	this->model = model;
}

Model* RenderComponent::getModel()
{
	return model;
}

void RenderComponent::setOverrideMaterial(Material* material)
{
	this->overrideMaterial = material;
}

Material* RenderComponent::getOverrideMaterial()
{
	return overrideMaterial;
}