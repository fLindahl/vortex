#pragma once
#include "foundation/math/line.h"
#include "foundation/math/matrix4.h"

namespace BaseGameFeature
{

class EnvManager
{
private:
	EnvManager()
	{

	}

public:
	static EnvManager* Instance()
	{
		static EnvManager instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	EnvManager(EnvManager const&) = delete;
	void operator=(EnvManager const&) = delete;

	Math::line ComputeMouseWorldRay(const double& cursorX, const double& cursorY, const float& length, const float& viewPortSizeX, const float& viewPortSizeY);

private:


};

}