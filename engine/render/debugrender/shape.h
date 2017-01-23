#pragma once
#include "GL/glew.h"
#include <foundation/math/vector4.h>
#include "render/resources/shaderobject.h"


namespace Debug
{
	enum DebugShape
	{
		LINE,
		SPHERE,
		BOX,
		CONE,
		CAPSULE,
		FRUSTUM,
		MESH,
		CIRCLE
	};

	enum RenderMode
	{
		Normal = 1,
		AlwaysOnTop = 2,
		WireFrame = 4
	};

	struct RenderCommand
	{
		DebugShape shape;
		char rendermode = RenderMode::Normal;
		float linewidth = 1.0f;
	};

	class RenderShape
	{
	public:
		RenderShape() {}
		virtual ~RenderShape() {}

		virtual void Draw(RenderCommand* command) = 0;
	
	protected:
		DebugShape shape;

		///Shader that this renderer uses
		std::shared_ptr<Render::ShaderObject> shader;

		GLuint vao[1];
		GLuint ib[1];
		GLuint vbo[1];
	};
}