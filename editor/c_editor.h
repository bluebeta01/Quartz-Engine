#pragma once
#include <engine/engine.h>
#include "c_gizmo.h"

class Editor
{
public:
	Engine* m_engine;
	Gizmo m_gizmo;
	Entity* m_camera;
	GameWindow* m_gameWindow;
	Editor();
	void tick();
	void render();
	void terminate();

private:
	void cameraMovement();
	glm::vec2 m_previousCursorPosition;
};