#include "framework/pch.h"
#include "c_entity.h"
#include "framework/render/render.h"

Entity::Entity()
{
	name = "noName";
}
Entity::Entity(std::string name, Transform transform)
{
	this->name = name;
	this->transform = transform;
}

void Entity::addComponent(Component* component)
{
	components.push_back(component);
}