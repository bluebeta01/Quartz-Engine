#pragma once
#include "c_component.h"
#include "render/c_camera.h"
class CameraComponent : public Component
{
public:
	Camera m_camera;
	CameraComponent();
	~CameraComponent();
	void update();
};