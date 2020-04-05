#include "pch.h"
#include "c_rendercomponent.h"

RenderComponent::RenderComponent(Model* model) : Component(COMPONENT_TYPE_RENDER_COMPONENT)
{
	setModel(model);
}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::update()
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