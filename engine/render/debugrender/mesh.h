#pragma once
#include <render/server/renderdevice.h>
#include "shape.h"
#include "render/resources/meshresource.h"

namespace Debug
{

struct MeshCommand : public RenderCommand
{
	Math::mat4 transform = Math::mat4::identity();
    Math::vec4 color = Math::vec4(1.0f);

	//Mesh to render
	std::shared_ptr<Render::MeshResource> mesh;
	//Primitive group to render. -1 renders all primitivegroups
	int primitiveGroup = -1;

	float lineWidth = 1.0f;
	bool wireframe = false;
};

class RenderMesh : public RenderShape
{
public:
	RenderMesh();
	~RenderMesh();

    void Draw(RenderCommand* command);
	
private:
    friend class DebugRenderer;

	
};

inline RenderMesh::RenderMesh()
{
    this->shape = DebugShape::MESH;
}

inline RenderMesh::~RenderMesh()
{

}

inline void RenderMesh::Draw(RenderCommand* command)
{
	MeshCommand* cmd = (MeshCommand*)command;

	glUseProgram(this->shader->GetProgram());

	cmd->mesh->Bind();

	GLuint loc = glGetUniformLocation(this->shader->GetProgram(), "color");
	glUniform4f(loc, cmd->color.x(), cmd->color.y(), cmd->color.z(), cmd->color.w());

	this->shader->setModelMatrix(cmd->transform);

	if (cmd->wireframe)
	{
		glDepthFunc(GL_LEQUAL);
		glPolygonMode(GL_FRONT, GL_LINE);
		glLineWidth(cmd->lineWidth);
	}

	if (cmd->primitiveGroup != -1)
	{
		cmd->mesh->Draw(cmd->primitiveGroup);
	}
	else
	{
		cmd->mesh->Draw();
	}

	if (cmd->wireframe)
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glDepthFunc(GL_LESS);
	}
    
	glBindVertexArray(0);
}

}