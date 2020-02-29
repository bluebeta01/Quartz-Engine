#pragma once
#include <unordered_map>
#include <string>
#include "ecs/entity/c_entity.h"
#include "transform.h"

class World
{
public:
	World();
	std::unordered_map<EntityUid, Entity*> m_entityMap;
	Entity* createEntity(std::string name, Transform transform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	void destoryEntity(Entity* entity);
	void destoryEntity(EntityUid entityUid);
private:

};