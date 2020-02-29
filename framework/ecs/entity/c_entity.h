#pragma once
#include "render/c_model.h"
#include "ecs/components/c_component.h"
#include <unordered_map>
#include <string>
#include "ecs/components/c_rendercomponent.h"
#include "transform.h"

typedef UINT EntityUid;

class Entity
{
public:
	Transform transform;
	std::string name;
	EntityUid m_uid;
	static EntityUid m_nextUid;
	std::unordered_map<Component::COMPONENT_TYPE, Component*> m_components;

	Entity(std::string name = "noName", Transform transform = Transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1,1,1)));
	~Entity();

	void addComponent(Component* component);
	Component* getComponent(Component::COMPONENT_TYPE componentType);
	void destroyComponent(Component::COMPONENT_TYPE componentType);
};