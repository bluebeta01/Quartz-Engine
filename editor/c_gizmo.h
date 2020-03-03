#pragma once
#include "framework/framework.h"

class Gizmo
{
public:
	Gizmo();
	Entity* m_selectedEntity = nullptr;
	void update();
private:
	Entity* m_gizmo = nullptr;
	Entity* m_transformGizmo;
	Entity* m_transformGizmoXArrow = nullptr;
	Entity* m_transformGizmoYArrow = nullptr;
	Entity* m_transformGizmoZArrow = nullptr;
	Entity* m_selectedGizmo = nullptr;
	glm::vec3 m_gizmoSelectLocation;

	void createGizmo();
	void selectEntity();
	void setupGizmo();
	void gizmoSelect();
	void gizmoDrag();
};