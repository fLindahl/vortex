#pragma once

namespace Debug
{
	class RenderShape
	{
	public:
		virtual ~RenderShape() = 0;

		virtual void Draw();
	
	protected:
		///Contains all vertices/indices
		void* mesh;
		void* indices;

		///PrimitiveGroups for this mesh
		//Util::Array<PrimitiveGroup> primitiveGroups;

		//GLuint vao[1];
		//GLuint ib[1];
		//GLuint vbo[1];


	};
}