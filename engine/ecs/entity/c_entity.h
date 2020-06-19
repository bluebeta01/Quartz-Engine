#pragma once
#include "render/c_model.h"
#include "ecs/components/c_component.h"
#include <unordered_map>
#include <string>
#include "ecs/components/c_rendercomponent.h"
#include "transform.h"

class World;

class Entity
{
public:
	std::string m_name;
	std::unordered_map<Component::COMPONENT_TYPE, Component*> m_components;
	std::vector<Entity*> m_children;
	Transform transform;
	World* m_world;
	EcsUid m_uid;
	static EcsUid s_nextUid;

	Entity(std::string name = "noName", Transform transform = Transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1,1,1)));
	~Entity();
	void destroyComponent(Component::COMPONENT_TYPE componentType);
	void setWorldPosition(glm::vec3 position);
	void removeChild(Entity* child);
	void addChild(Entity* child);
	glm::vec3 getWorldPosition();
	glm::mat4 getModelMatrix();
	Component* addComponent(Component* component);
	Component* getComponent(Component::COMPONENT_TYPE componentType);
	Entity* getParent();

private:
	Entity* m_parent = nullptr;
};