#include "framework/pch.h"
#include "c_entity_staticmesh.h"
#include "framework/common/components/c_rendercomponent.h"
#include "framework/render/render.h"

StaticMeshEntity::StaticMeshEntity(std::string name, Transform transform)
	: Entity(name, transform)
{
	RenderComponent* rc = new RenderComponent();
	this->addComponent(rc);
	//rc->setModel(game.renderManager.renderer->getModel(modelName));
}


StaticMeshEntity::~StaticMeshEntity()
{
}
