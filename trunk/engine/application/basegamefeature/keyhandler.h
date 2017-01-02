#pragma once
#include "render/window.h"

namespace BaseGameFeature
{

	class KeyHandler
	{
	private:
		KeyHandler()
		{
		
		}

	public:
		static KeyHandler* Instance()
		{
			static KeyHandler instance;
			return &instance;
		}

		// C++ 11
		// Delete the methods we don't want.
		KeyHandler(KeyHandler const&) = delete;
		void operator=(KeyHandler const&) = delete;

		void Init(Display::Window* display);

	//private:
		void setKeyStatePressed(int32 key);
		void setKeyStateReleased(int32 key);

		bool leftMousePressed = false;
		bool rightMousePressed = false;

		float64 mouseX = 0;
		float64 mouseY = 0;

		float64 oldx = 0;
		float64 oldy = 0;

		float64 scroll = 0;

		bool tUP = false;
		bool tDOWN = false;
		bool tLEFT = false;
		bool tRIGHT = false;

		bool W = false;
		bool S = false;
		bool A = false;
		bool D = false;
		bool Z = false;
		bool Y = false;

		bool leftShift = false;
		bool leftCtrl = false;
		

		bool quit = false;

	};


	inline void KeyHandler::Init(Display::Window* window)
	{
		//Setup keyboard and mouse functions
		window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32){
		if (action == GLFW_PRESS)
		this->setKeyStatePressed(key);
		else if (action == GLFW_RELEASE)
		this->setKeyStateReleased(key);
		});
		window->SetMousePressFunction([this](int32 key, int32 action, int32){
		if (action == GLFW_PRESS)
		this->setKeyStatePressed(key);
		if (action == GLFW_RELEASE)
		this->setKeyStateReleased(key);
		});
		window->SetMouseMoveFunction([this](float64 y, float64 x){
		this->mouseX += x - oldx;
		this->mouseY += y - oldy;
		oldx = x;
		oldy = y;
		});
		window->SetMouseScrollFunction([this](float64, float64 y){
		this->scroll += y;
		});
	}

	inline void KeyHandler::setKeyStatePressed(int32 key)
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
		if (key == GLFW_KEY_LEFT_SHIFT)
		{
			leftShift = true;
		}
		if (key == GLFW_KEY_LEFT_CONTROL)
		{
			leftCtrl = true;
		}
		if (key == GLFW_KEY_Z)
		{
			Z = true;
		}
		if (key == GLFW_KEY_Y)
		{
			Y = true;
		}
	}

	inline void KeyHandler::setKeyStateReleased(int32 key)
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
		if (key == GLFW_KEY_LEFT_SHIFT)
		{
			leftShift = false;
		}
		if (key == GLFW_KEY_LEFT_CONTROL)
		{
			leftCtrl = false;
		}
		if (key == GLFW_KEY_Z)
		{
			Z = false;
		}
		if (key == GLFW_KEY_Y)
		{
			Y = false;
		}
	}
}