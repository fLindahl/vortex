#pragma once
#include "GL/glew.h"
#include <foundation/math/vector4.h>


namespace Debug
{
	enum DebugShape
	{
		POINT,
		LINE,
		SPHERE,
		BOX,
		CONE,
		CAPSULE,
		FRUSTUM
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

		/// Override and set static size
		static const int meshSize = 0;
		Math::vec4 mesh[0];

		/// Override and set static size
		static const int indicesSize = 0;
		int indices[0];

		GLuint vao[1];
		GLuint ib[1];
		GLuint vbo[1];
	};
}