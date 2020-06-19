#pragma once
#include <unordered_map>
#include <string>
#include "ecs/entity/c_entity.h"
#include "transform.h"

class World
{
public:
	std::unordered_map<EcsUid, Entity*> m_entityMap;

	void destoryEntity(Entity* entity);
	Entity* createEntity(std::string name, Transform transform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
private:

};