#pragma once
#include "c_component.h"
#include "render/c_model.h"
#include "render/c_material.h"

class RenderComponent : public Component
{
public:
	bool m_onTop = false;

	RenderComponent(Model* model);
	void setModel(Model* model);
	void setOverrideMaterial(Material* material);
	Model* getModel();
	Material* getOverrideMaterial();

private:
	Model * model = NULL;
	Material * overrideMaterial = NULL;
};