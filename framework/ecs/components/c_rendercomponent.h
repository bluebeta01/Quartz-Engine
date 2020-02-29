#pragma once
#include "c_component.h"
#include "render/c_model.h"
#include "render/c_material.h"

class RenderComponent : public Component
{
public:
	RenderComponent();
	~RenderComponent();
	void setModel(Model* model);
	Model* getModel();
	void setOverrideMaterial(Material* material);
	Material* getOverrideMaterial();

private:
	Model * model = NULL;
	Material * overrideMaterial = NULL;
};