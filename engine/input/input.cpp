#include "pch.h"
#include "input.h"

namespace input
{
	glm::vec2 cursorPosition;
	glm::vec2 cursorDelta;
	bool cursorLock = false;
	glm::vec2 cursorLockPosition;
	std::vector<int> keysDown;
	std::vector<int> keysPressed;

	std::vector<MOUSEBUTTON> mouseButtonsDown;
	std::vector<MOUSEBUTTON> mouseButtonsClicked;

	void registerKeyDown(int key)
	{
		for (int i = 0; i < keysDown.size(); i++)
		{
			if (keysDown[i] == key)
			{
				return;
			}
		}
		keysDown.push_back(key);
	}
	void registerKeyUp(int key)
	{
		for (int i = 0; i < keysDown.size(); i++)
		{
			if (keysDown[i] == key)
			{
				keysDown.erase(keysDown.begin() + i);
			}
		}
	}
	void registerKeyPressed(int key)
	{
		for (int i = 0; i < keysPressed.size(); i++)
		{
			if (keysPressed[i] == key)
			{
				return;
			}
		}
		keysPressed.push_back(key);
	}
	void clearKeysPressed()
	{
		keysPressed.clear();
	}
	bool isKeyDown(int key)
	{
		for (int i = 0; i < keysDown.size(); i++)
		{
			if (keysDown[i] == key)
				return true;
		}
		return false;
	}
	bool isKeyPressed(int key)
	{
		for (int i = 0; i < keysPressed.size(); i++)
		{
			if (keysPressed[i] == key)
				return true;
		}
		return false;
	}

	void registerMouseButtonDown(MOUSEBUTTON button)
	{
		if (!isMouseButtonDown(button))
			mouseButtonsDown.push_back(button);
	}
	void registerMouseButtonUp(MOUSEBUTTON button)
	{
		for (int i = 0; i < mouseButtonsDown.size(); i++)
		{
			if (mouseButtonsDown[i] == button)
			{
				mouseButtonsDown.erase(mouseButtonsDown.begin() + i);
			}
		}
	}
	void registerMouseButtonClick(MOUSEBUTTON button)
	{
		if (!isMouseButtonClicked(button))
			mouseButtonsClicked.push_back(button);
	}
	void clearMouseButtonsClicked()
	{
		mouseButtonsClicked.clear();
	}
	bool isMouseButtonDown(MOUSEBUTTON button)
	{
		for (int i = 0; i < mouseButtonsDown.size(); i++)
		{
			if (mouseButtonsDown[i] == button)
				return true;
		}
		return false;
	}
	bool isMouseButtonClicked(MOUSEBUTTON button)
	{
		for (int i = 0; i < mouseButtonsClicked.size(); i++)
		{
			if (mouseButtonsDown[i] == button)
				return true;
		}
		return false;
	}
}