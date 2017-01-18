#pragma once
#include <render/server/renderdevice.h>
#include "shape.h"

namespace Debug
{
	struct CircleCommand : public RenderCommand
	{
		Math::mat4 transform = Math::mat4::identity();
		Math::vec4 color = Math::vec4(1.0f);
	};

	class RenderCircle : public RenderShape
	{
	public:
		RenderCircle();
		~RenderCircle();

		void Draw(RenderCommand* command);

	private:
		friend class DebugRenderer;

		void SetupBuffers();
	};

	inline RenderCircle::RenderCircle()
	{
		this->shape = DebugShape::CIRCLE;
		this->SetupBuffers();
	}

	inline RenderCircle::~RenderCircle()
	{

	}

	inline void RenderCircle::SetupBuffers()
	{

		const int meshSize = 63;

		const GLfloat mesh[meshSize] =
		{
			0.951057f, 0.000000f, -0.309017f,
			0.809018f, 0.000000f, -0.587786f,
			0.587786f, 0.000000f, -0.809017f,
			0.309017f, 0.000000f, -0.951057f,
			0.000000f, 0.000000f, -1.000000f,
			-0.309017f, 0.000000f, -0.951057f,
			-0.587785f, 0.000000f, -0.809017f,
			-0.809017f, 0.000000f, -0.587785f,
			-0.951057f, 0.000000f, -0.309017f,
			-1.000000f, 0.000000f, 0.000000f,
			-0.951057f, 0.000000f, 0.309017f,
			-0.809017f, 0.000000f, 0.587785f,
			-0.587785f, 0.000000f, 0.809017f,
			-0.309017f, 0.000000f, 0.951057f,
			-0.000000f, 0.000000f, 1.000000f,
			0.309017f, 0.000000f, 0.951057f,
			0.587785f, 0.000000f, 0.809017f,
			0.809017f, 0.000000f, 0.587785f,
			0.951057f, 0.000000f, 0.309017f,
			1.000000f, 0.000000f, 0.000000f,
			0.000000f, 0.000000f, 0.000000f
		};

		const int indicesSize = 100;
		const int indices[] =
		{
			//Filled Circe
			0, 1, 20,
			1, 2, 20,
			2, 3, 20,
			3, 4, 20,
			4, 5, 20,
			5, 6, 20,
			6, 7, 20,
			7, 8, 20,
			8, 9, 20,
			9, 10, 20,
			10, 11, 20,
			11, 12, 20,
			12, 13, 20,
			13, 14, 20,
			14, 15, 20,
			15, 16, 20,
			16, 17, 20,
			17, 18, 20,
			18, 19, 20,
			19, 0, 20,
			

			//Outlines (offset 60)
			0, 1,
			1, 2,
			2, 3,
			3, 4,
			4, 5,
			5, 6,
			6, 7,
			7, 8,
			8, 9,
			9, 10,
			10, 11,
			11, 12,
			12, 13,
			13, 14,
			14, 15,
			15, 16,
			16, 17,
			17, 18,
			18, 19,
			19, 0,
			
			
			
			

		};

		glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object
		glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it

		glGenBuffers(1, this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, meshSize * sizeof(GLfloat), &mesh[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, NULL);

		glGenBuffers(1, this->ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ib[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	inline void RenderCircle::Draw(RenderCommand* command)
	{
		CircleCommand* cmd = (CircleCommand*)command;

		glUseProgram(this->shader->GetProgram());

		glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it

		GLuint loc = glGetUniformLocation(this->shader->GetProgram(), "color");
		glUniform4f(loc, cmd->color.x(), cmd->color.y(), cmd->color.z(), cmd->color.w());

		this->shader->setModelMatrix(cmd->transform);

		if ((cmd->rendermode & RenderMode::AlwaysOnTop) == RenderMode::AlwaysOnTop)
		{
			glDepthFunc(GL_ALWAYS);
		}

		if ((cmd->rendermode & RenderMode::WireFrame) == RenderMode::WireFrame)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glLineWidth(cmd->linewidth);

			glDrawElements(GL_LINES, 40, GL_UNSIGNED_INT, (void*)(60 * sizeof(GLuint)));
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		else
		{
			glDisable(GL_CULL_FACE);
			glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, NULL);
			glEnable(GL_CULL_FACE);
		}

		if ((cmd->rendermode & RenderMode::AlwaysOnTop) == RenderMode::AlwaysOnTop)
		{
			glDepthFunc(GL_LESS);
		}
	}


}