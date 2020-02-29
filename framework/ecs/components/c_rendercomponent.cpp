#include "pch.h"
#include "c_rendercomponent.h"

RenderComponent::RenderComponent() : Component()
{
	m_componentType = COMPONENT_TYPE_RENDER_COMPONENT;
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