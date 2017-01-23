#pragma once
#include "GL/glew.h"


namespace Debug
{
class DebugServer
{
private:
	DebugServer();
	
public:
	static DebugServer* Instance()
	{
		static DebugServer instance;
		return &instance;
	}

	DebugServer(DebugServer const&) = delete;
	void operator=(DebugServer const&) = delete;
	
	void ImGuiDebugBar();

private:
	


};
}