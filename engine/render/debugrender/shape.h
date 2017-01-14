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
		MESH
	};

	struct RenderCommand
	{
		DebugShape shape;
		//TODO: Add render on top
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