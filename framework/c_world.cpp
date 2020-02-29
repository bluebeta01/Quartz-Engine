#include "pch.h"
#include "c_world.h"

World::World()
{

}

Entity* World::createEntity(std::string name, Transform transform)
{
	Entity* e = new Entity(name, transform);
	m_entityMap[e->m_uid] = e;
	return e;
}

void World::destoryEntity(Entity* entity)
{
	m_entityMap.erase(entity->m_uid);
	delete entity;
}

void World::destoryEntity(EntityUid entityUid)
{
	Entity* e = m_entityMap[entityUid];
	if (!e)
		return;
	delete e;
	m_entityMap.erase(entityUid);
}