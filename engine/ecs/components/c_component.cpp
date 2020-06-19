#pragma once
#include "pch.h"
#include "c_component.h"
#include "ecs/entity/c_entity.h"

std::unordered_map<EcsUid, Component*> Component::s_components;

Component::Component(COMPONENT_TYPE type) : m_componentType(type)
{
	m_uid = Entity::s_nextUid++;
}