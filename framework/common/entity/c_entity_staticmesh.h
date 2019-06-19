#pragma once
#include "c_entity.h"

class StaticMeshEntity : public Entity
{
public:
	StaticMeshEntity(std::string name, Transform transform);
	~StaticMeshEntity();
};

