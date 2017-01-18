#pragma once
#include <render/server/renderdevice.h>
#include "shape.h"

namespace Debug
{

struct LineCommand : public RenderCommand
{
    Math::vec4 startpoint = Math::point(0.0f);
    Math::vec4 endpoint = Math::point(1.0f);
    Math::vec4 startcolor = Math::vec4(1.0f);
    Math::vec4 endcolor = Math::vec4(1.0f);

};

class RenderLine : public RenderShape
{
public:
    RenderLine();
    ~RenderLine();

    void Draw(RenderCommand* command);

private:
    friend class DebugRenderer;

    void SetupBuffers();

    ///Vertex structure is (vec4 pos, vec4 color)
    static const int meshSize = 4;
	Math::vec4 mesh[4];
};

inline RenderLine::RenderLine()
{
    this->shape = DebugShape::LINE;
    this->SetupBuffers();
}

inline RenderLine::~RenderLine()
{

}

inline void RenderLine::SetupBuffers()
{
    glGenVertexArrays(1, &vao[0]); // Create our Vertex Array Object
    glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it

    glGenBuffers(1, this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, this->meshSize * sizeof(Math::vec4), &mesh[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(4 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline void RenderLine::Draw(RenderCommand* command)
{
    LineCommand* lineCommand = (LineCommand*)command;

	glUseProgram(this->shader->GetProgram());

	if ((lineCommand->rendermode & RenderMode::AlwaysOnTop) == RenderMode::AlwaysOnTop)
	{
		glDepthFunc(GL_ALWAYS);
		glDepthRange(0.0f,0.01f);
	}

	glPolygonMode(GL_FRONT, GL_LINE);
	
    glLineWidth(lineCommand->linewidth);

    glBindVertexArray(vao[0]); // Bind our Vertex Array Object so we can use it

    //change vertices and update mesh buffer
    this->mesh[0] = lineCommand->startpoint;
    this->mesh[1] = lineCommand->startcolor;

    this->mesh[2] = lineCommand->endpoint;
    this->mesh[3] = lineCommand->endcolor;

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, this->meshSize * sizeof(Math::vec4), &mesh[0], GL_DYNAMIC_DRAW);
	
	glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPolygonMode(GL_FRONT, GL_FILL);

	if ((lineCommand->rendermode & RenderMode::AlwaysOnTop) == RenderMode::AlwaysOnTop)
	{
		glDepthFunc(GL_LESS);
		glDepthRange(0.0f, 1.0f);
	}

}

}