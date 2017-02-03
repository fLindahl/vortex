#pragma once
#include <render/server/renderdevice.h>
#include "shape.h"

namespace Debug
{
struct ConeCommand: public RenderCommand
{
	Math::mat4 transform = Math::mat4::identity();
	Math::vec4 color = Math::vec4(1.0f);
};

class RenderCone : public RenderShape
{
public:
	RenderCone();
	~RenderCone();

	void Draw(RenderCommand* command);

private:
	friend class DebugRenderer;

	void SetupBuffers();
};

inline RenderCone::RenderCone()
{
	this->shape = DebugShape::CONE;
	this->SetupBuffers();
}

inline RenderCone::~RenderCone()
{
	
}

inline void RenderCone::SetupBuffers()
{
	const int meshSize = 63;

	const GLfloat mesh[meshSize] =
	{
		0.951057f, -1.000000f, -0.309017f,
		0.809018f, -1.000000f, -0.587786f,
		0.587786f, -1.000000f, -0.809017f,
		0.309017f, -1.000000f, -0.951057f,
		0.000000f, -1.000000f, -1.000000f,
		-0.309017f, -1.000000f, -0.951057f,
		-0.587785f, -1.000000f, -0.809017f,
		-0.809017f, -1.000000f, -0.587785f,
		-0.951057f, -1.000000f, -0.309017f,
		-1.000000f, -1.000000f, 0.000000f,
		-0.951057f, -1.000000f, 0.309017f,
		-0.809017f, -1.000000f, 0.587785f,
		-0.587785f, -1.000000f, 0.809017f,
		-0.309017f, -1.000000f, 0.951057f,
		-0.000000f, -1.000000f, 1.000000f,
		0.309017f, -1.000000f, 0.951057f,
		0.587785f, -1.000000f, 0.809017f,
		0.809017f, -1.000000f, 0.587785f,
		0.951057f, -1.000000f, 0.309017f,
		1.000000f, -1.000000f, 0.000000f,
		0.000000f, 1.000000f, 0.000000f
	};

	const int indicesSize = 162;
	const int indices[] =
	{
		//Filled Cone
		0, 19, 1,
		19, 18, 1,
		18, 17, 1,
		17, 16, 1,
		16, 15, 1,
		15, 14, 1,
		14, 13, 1,
		13, 12, 1,
		12, 11, 1,
		11, 10, 1,
		10, 9, 1,
		9, 8, 1,
		8, 7, 1,
		7, 6, 1,
		6, 5, 1,
		5, 4, 1,
		4, 3, 1,
		3, 2, 1,
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


		//Outlines (offset 114)
		0, 19,
		19, 18,
		18, 17,
		17, 16,
		16, 15,
		15, 14,
		14, 13,
		13, 12,
		12, 11,
		11, 10,
		10, 9,
		9, 8,
		8, 7,
		7, 6,
		6, 5,
		5, 4,
		4, 3,
		3, 2,
		2, 1,
		1, 0,
		4, 20,
		9, 20,
		14, 20,
		19, 20

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

inline void RenderCone::Draw(RenderCommand* command)
{
	ConeCommand* cmd = (ConeCommand*)command;

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

		glDrawElements(GL_LINES, 48, GL_UNSIGNED_INT, (void*)(114 * sizeof(GLuint)));
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, 114, GL_UNSIGNED_INT, NULL);
	}

	if ((cmd->rendermode & RenderMode::AlwaysOnTop) == RenderMode::AlwaysOnTop)
	{
		glDepthFunc(GL_LESS);
	}
}


}