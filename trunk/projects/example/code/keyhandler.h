#pragma once
#include "config.h"

namespace KeyHandler
{
	class KeyHandler
	{
	public:
		KeyHandler();
		~KeyHandler();

		void setKeyStatePressed(int32 key);
		void setKeyStateReleased(int32 key);

		bool leftMousePressed = false;
		bool rightMousePressed = false;

		bool tUP = false;
		bool tDOWN = false;
		bool tLEFT = false;
		bool tRIGHT = false;
		
		bool W = false;
		bool S = false;
		bool A = false;
		bool D = false;
		
		bool quit = false;

	private:


	};
}