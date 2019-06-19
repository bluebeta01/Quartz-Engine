#pragma once
#include "framework/pch.h"
#include "framework/render/c_model.h"
#include "framework/common/components/c_component.h"

class Entity
{
public:
	Transform transform;
	std::string name;

	Entity();
	Entity(std::string name, Transform transform);

	template <class TComponent>
	TComponent getComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			TComponent tc = dynamic_cast<TComponent>(components[i]);
			if (tc)
				return tc;
		}

		return NULL;
	}

	void addComponent(Component* component);

private:
	std::vector<Component*> components;
};