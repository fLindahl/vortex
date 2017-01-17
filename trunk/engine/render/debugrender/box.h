#pragma once
#include <render/server/renderdevice.h>
#include "shape.h"

namespace Debug
{

struct BoxCommand : public RenderCommand
{
	Math::mat4 transform = Math::mat4::identity();
    Math::vec4 color = Math::vec4(1.0f);
};

class RenderBox : public RenderShape
{
public:
	RenderBox();
	~RenderBox();

    void Draw(RenderCommand* command);

private:
    friend class DebugRenderer;

    void SetupBuffers();
};

inline RenderBox::RenderBox()
{
    this->shape = DebugShape::BOX;
    this->SetupBuffers();
}

inline RenderBox::~RenderBox()
{

}

inline void RenderBox::SetupBuffers()
{
	const int meshSize = 24;

	const GLfloat mesh[meshSize] =
	{
		0.5, -0.5, -0.5,
		0.5, -0.5, 0.5,
		-0.5, -0.5, 0.5,
		-0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5,
		-0.5, 0.5, -0.5
	};

	const int indicesSize = 60;
	const int indices[] =
	{
		//Triangles
		0, 1, 2,
		3, 0, 2,
		4, 7, 6,
		5, 4, 6,
		0, 4, 5,
		1, 0, 5,
		1, 5, 6,
		2, 1, 6,
		2, 6, 7,
		3, 2, 7,
		4, 0, 3,
		7, 4, 3,

		//Outlines (offset 36)
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
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

inline void RenderBox::Draw(RenderCommand* command)
{
    BoxCommand* cmd = (BoxCommand*)command;

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

		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
		glPolygonMode(GL_FRONT, GL_FILL);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	}

	if ((cmd->rendermode & RenderMode::AlwaysOnTop) == RenderMode::AlwaysOnTop)
	{
		glDepthFunc(GL_LESS);
	}
    
	glBindVertexArray(0);
}

}