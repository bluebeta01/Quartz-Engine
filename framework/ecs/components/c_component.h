#pragma once
//#include "ecs/entity/c_entity.h"

class Entity;

class Component
{
public:
	enum COMPONENT_TYPE { COMPONENT_TYPE_NONE,
		COMPONENT_TYPE_RENDER_COMPONENT };
	COMPONENT_TYPE m_componentType = COMPONENT_TYPE_NONE;
	Entity* m_entity;
	virtual ~Component() {}
};