#pragma once
#include "core/refcounted.h"
#include "foundation/util/string.h"
#include "foundation/util/array.h"
#include "render/resources/material.h"

namespace Render
{	
	class FramePass : public Core::RefCounted
	{
	__DeclareClass(FramePass);

	public:
		FramePass();
		virtual ~FramePass();
		
        virtual void Setup();

		virtual void Execute();

	protected:
		friend class RenderDevice;
		friend class FrameServer;
		Util::String name;
	};

}
