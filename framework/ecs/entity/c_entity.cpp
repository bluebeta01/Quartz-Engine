#include "pch.h"
#include "c_entity.h"
#include "render/render.h"

UINT Entity::m_nextUid = 0;

Entity::Entity(std::string name, Transform transform)
{
	this->m_name = name;
	this->transform = transform;

	m_uid = m_nextUid++;
}

Entity::~Entity()
{

}

void Entity::addComponent(Component* component)
{
	Component* c = m_components[component->m_componentType];
	if (c)
	{
		LOGWARNING("Cannot add multiple components of the same type to an entity!");
		delete component;
		return;
	}
	m_components[component->m_componentType] = component;
}

Component* Entity::getComponent(Component::COMPONENT_TYPE componentType)
{
	return m_components[componentType];
}

void Entity::destroyComponent(Component::COMPONENT_TYPE componentType)
{
	Component* c = m_components[componentType];
	if (!c)
		return;
	delete c;
	m_components.erase(componentType);
}

glm::vec3 Entity::getWorldPosition()
{
	if (!m_parent)
		return transform.position;

	glm::vec3 parentPosition = m_parent->getWorldPosition();
	glm::vec3 rotationVector = glm::vec3(glm::radians(-m_parent->transform.rotation.x),
		glm::radians(-m_parent->transform.rotation.y), glm::radians(-m_parent->transform.rotation.z));
	glm::vec3 adjustedPosition = transform.position + m_parent->getWorldPosition();
	
	//http://mathforum.org/library/drmath/view/69806.html
	
	glm::vec3 a = adjustedPosition - parentPosition;
	glm::vec4 b = glm::vec4(a, 1.0f);
	glm::vec4 c = b * glm::rotate(rotationVector.x, glm::vec3(1, 0, 0)) * glm::rotate(rotationVector.y, glm::vec3(0, 1, 0)) * glm::rotate(rotationVector.z, glm::vec3(0, 0, 1));
	glm::vec3 d = glm::vec3(c.x, c.y, c.z);
	glm::vec3 e = d + parentPosition;
	return e;
}

glm::mat4 Entity::getModelMatrix()
{
	Transform t;
	t.position = getWorldPosition();
	t.rotation = transform.rotation;
	if (m_parent)
		t.rotation = transform.rotation + m_parent->transform.rotation;
	t.scale = transform.scale;
	return Transform::matrixFromTransform(t, true);
}

void Entity::setWorldPosition(glm::vec3 position)
{
	if (!m_parent)
	{
		transform.position = position;
		return;
	}

	glm::vec3 moveAmount = position - m_parent->transform.position;

	glm::mat4 parentRotMat = glm::rotate(glm::radians(-m_parent->transform.rotation.x), glm::vec3(1, 0, 0)) * 
		glm::rotate(glm::radians(-m_parent->transform.rotation.y), glm::vec3(0, 1, 0)) * 
		glm::rotate(glm::radians(-m_parent->transform.rotation.z), glm::vec3(0, 0, 1));

	parentRotMat = glm::inverse(parentRotMat);

	glm::vec4 moveAmount4 = glm::vec4(moveAmount, 1.0f) * parentRotMat;
	moveAmount = glm::vec3(moveAmount4.x, moveAmount4.y, moveAmount4.z);

	transform.position = moveAmount;
}

Entity* Entity::getParent()
{
	return m_parent;
}

void Entity::removeChild(Entity* child)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i] == child)
		{
			m_children.erase(m_children.begin() + i);
			child->m_parent = nullptr;
		}
	}
}

void Entity::addChild(Entity* child)
{
	m_children.push_back(child);
	child->m_parent = this;
}