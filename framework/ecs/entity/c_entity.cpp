#include "pch.h"
#include "c_entity.h"
#include "render/render.h"

UINT Entity::m_nextUid = 0;

Entity::Entity(std::string name, Transform transform)
{
	this->name = name;
	this->transform = transform;

	m_uid = m_nextUid++;
}

Entity::~Entity()
{

}

void Entity::addComponent(Component* component)
{
	Component* c = m_components[component->m_componentType];
	if (c)
	{
		LOGWARNING("Cannot add multiple components of the same type to an entity!");
		delete component;
		return;
	}
	m_components[component->m_componentType] = component;
}

Component* Entity::getComponent(Component::COMPONENT_TYPE componentType)
{
	return m_components[componentType];
}

void Entity::destroyComponent(Component::COMPONENT_TYPE componentType)
{
	Component* c = m_components[componentType];
	if (!c)
		return;
	delete c;
	m_components.erase(componentType);
}