#pragma once
#include "engine/engine.h"

class Gizmo
{
public:
	Gizmo();
	Entity* m_selectedEntity = nullptr;

	Renderer* renderer;
	Camera* camera;
	Framebuffer* frameBuffer;
	World* world;

	void update();
	void initialize(Renderer* renderer, Camera* camera, World* world);
	Entity* m_gizmo = nullptr;
private:
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