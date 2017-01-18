#pragma once
#include "render/properties/graphicsproperty.h"

namespace Render
{
	class VisibilityServer
	{
	private:
		VisibilityServer();

	public:
		static VisibilityServer* Instance()
		{
			static VisibilityServer instance;
			return &instance;
		}
		
		VisibilityServer(const VisibilityServer&) = delete;
		void operator=(const VisibilityServer&) = delete;

		void UpdatePVS();

	private:
		//Potetially visible set of Graphics Nodes
		//TODO: This should be a multidimensional list of all materials and their potentially visible nodes
		Util::Array<GraphicsProperty*> PVSet;

		

	};
}