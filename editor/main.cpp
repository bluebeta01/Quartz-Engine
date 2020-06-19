#include "c_editor.h"
#include "engine/engine.h"

int main()
{
	Editor editor;

	while (!editor.m_gameWindow->s_isTerminating)
	{
		editor.tick();
		editor.render();
	}

	editor.terminate();
}