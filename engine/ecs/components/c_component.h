#pragma once
#include <unordered_map>

class Entity;

typedef UINT EcsUid;

class Component
{
public:
	enum COMPONENT_TYPE { COMPONENT_TYPE_NONE,
		COMPONENT_TYPE_RENDER_COMPONENT,
		COMPONENT_TYPE_CAMERA_COMPONENT};
	COMPONENT_TYPE m_componentType = COMPONENT_TYPE_NONE;
	Entity* m_entity;
	bool enabled = true;
	EcsUid m_uid;
	static std::unordered_map<EcsUid, Component*> s_components;

	Component(COMPONENT_TYPE type);
	virtual void update() {};
	virtual ~Component() {};
};