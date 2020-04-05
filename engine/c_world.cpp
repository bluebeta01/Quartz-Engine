#include "pch.h"
#include "c_world.h"

World::World()
{

}

Entity* World::createEntity(std::string name, Transform transform)
{
	Entity* e = new Entity(name, transform);
	e->m_world = this;
	m_entityMap[e->m_uid] = e;
	return e;
}

void World::destoryEntity(Entity* entity)
{
	m_entityMap.erase(entity->m_uid);
	delete entity;
}
