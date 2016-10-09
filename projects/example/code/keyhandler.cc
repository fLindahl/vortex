#include "keyhandler.h"
#include <GLFW/glfw3.h>
namespace KeyHandler
{

	KeyHandler::KeyHandler()
	{

	}

	KeyHandler::~KeyHandler()
	{

	}

	void KeyHandler::setKeyStatePressed(int32 key)
	{
		if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rightMousePressed = true;
		}
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			leftMousePressed = true;
		}
		if (key == GLFW_KEY_UP)
		{
			tUP = true;
		}
		if (key == GLFW_KEY_DOWN)
		{
			tDOWN = true;
		}
		if (key == GLFW_KEY_LEFT)
		{
			tLEFT = true;
		}
		if (key == GLFW_KEY_RIGHT)
		{
			tRIGHT = true;
		}
		if (key == GLFW_KEY_W)
		{
			W = true;
		}
		if (key == GLFW_KEY_S)
		{
			S = true;
		}
		if (key == GLFW_KEY_A)
		{
			A = true;
		}
		if (key == GLFW_KEY_D)
		{
			D = true;
		}
	}

	void KeyHandler::setKeyStateReleased(int32 key)
	{
		if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rightMousePressed = false;
		}
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			leftMousePressed = false;
		}
		if (key == GLFW_KEY_UP)
		{
			tUP = false;
		}
		if (key == GLFW_KEY_DOWN)
		{
			tDOWN = false;
		}
		if (key == GLFW_KEY_LEFT)
		{
			tLEFT = false;
		}
		if (key == GLFW_KEY_RIGHT)
		{
			tRIGHT = false;
		}
		if (key == GLFW_KEY_W)
		{
			W = false;
		}
		if (key == GLFW_KEY_S)
		{
			S = false;
		}
		if (key == GLFW_KEY_A)
		{
			A = false;
		}
		if (key == GLFW_KEY_D)
		{
			D = false;
		}
		if (key == GLFW_KEY_ESCAPE)
		{
			quit = true;
		}
	}
}
