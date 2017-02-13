#include "config.h"
#include "debugrenderer.h"


namespace Debug
{

DebugRenderer::DebugRenderer()
{
    //commandQueue = Util::Queue<RenderCommand>();
}

void DebugRenderer::Initialize()
{
    this->line.shader = Render::ShaderServer::Instance()->LoadShader("debugLines");
	this->box.shader = Render::ShaderServer::Instance()->LoadShader("debug");
	this->mesh.shader = Render::ShaderServer::Instance()->LoadShader("debug");
	this->cone.shader = Render::ShaderServer::Instance()->LoadShader("debug");
	this->circle.shader = Render::ShaderServer::Instance()->LoadShader("debug");

    return;
}

void DebugRenderer::DrawLine(const Math::point &startPoint,
                             const Math::point &endPoint,
                             const float &lineWidth,
                             const Math::vec4 &startColor,
							 const Math::vec4 &endColor, 
							 const RenderMode& renderModes)
{
    LineCommand* cmd = new LineCommand();
    cmd->shape = DebugShape::LINE;
    cmd->startpoint = startPoint;
    cmd->endpoint = endPoint;
    cmd->linewidth = lineWidth;
	cmd->rendermode = renderModes;
    cmd->startcolor = startColor;
    cmd->endcolor = endColor;

    this->commandQueue.push(cmd);
}


void DebugRenderer::DrawBox(const Math::vector& position,
							const Math::quaternion& rotation, 
							const float& scale, 
							const Math::vec4& color, 
							const RenderMode& renderModes,
							const float& lineWidth)
{
	BoxCommand* cmd = new BoxCommand();
	cmd->shape = DebugShape::BOX;

	Math::mat4 transform = Math::mat4::multiply(Math::mat4::scaling(scale), Math::mat4::rotationquaternion(rotation));
	transform.translate(position);

	cmd->transform = transform;
		
	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);

}

void DebugRenderer::DrawBox(const Math::vector& position, 
							const Math::quaternion& rotation, 
							const float& width, 
							const float& height, 
							const float& length, 
							const Math::vec4& color, 
							const RenderMode& renderModes,
							const float& lineWidth)
{
	BoxCommand* cmd = new BoxCommand();
	cmd->shape = DebugShape::BOX;

	Math::mat4 transform = Math::mat4::multiply(Math::mat4::scaling(Math::point(width, height, length)), Math::mat4::rotationquaternion(rotation));
	transform.translate(position);

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);
}

void DebugRenderer::DrawBox(const Math::mat4& transform, 
							const Math::vec4& color, 
							const RenderMode& renderModes,
							const float& lineWidth)
{
	BoxCommand* cmd = new BoxCommand();
	cmd->shape = DebugShape::BOX;

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);
}

void DebugRenderer::DrawBox(const Math::bbox& bbox, 
							const Math::vec4& color, 
							const RenderMode& renderModes,
							const float& lineWidth)
{
	BoxCommand* cmd = new BoxCommand();
	cmd->shape = DebugShape::BOX;

	float width = bbox.get_maxx() - bbox.get_minx();
	float height = bbox.get_maxy() - bbox.get_miny();
	float length = bbox.get_maxz() - bbox.get_minz();
	
	Math::vector position = (bbox.get_minpoint() + bbox.get_maxpoint()) * 0.5f;

	Math::mat4 transform = Math::mat4::scaling(Math::point(width, height, length));
	transform.translate(position);

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	//Always draw bboxes with wireframe
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);
}

void DebugRenderer::DrawCone(const Math::vector& position,
							 const Math::quaternion& rotation, 
							 const float& radius, 
							 const float& length, 
							 const Math::vec4& color, 
							 const RenderMode& renderModes, 
							 const float& lineWidth)
{
	ConeCommand* cmd = new ConeCommand();
	cmd->shape = DebugShape::CONE;
	Math::mat4 dirTrans = Math::mat4::rotationquaternion(rotation);
	Math::vector dir = Math::vector::normalize3(dirTrans.get_yaxis());
	Math::mat4 transform = Math::mat4::multiply(Math::mat4::scaling(Math::point(radius, length/2, radius)), dirTrans);
	transform.translate(position);
	transform.translate(-dir * (length/2));

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);

}

void DebugRenderer::DrawCone(const Math::mat4& transform,
							 const Math::vec4& color,
							 const RenderMode& renderModes,
							 const float& lineWidth)
{
	ConeCommand* cmd = new ConeCommand();
	cmd->shape = DebugShape::CONE;
	
	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);

}

void DebugRenderer::DrawCircle(const Math::vector& position, const Math::quaternion& rotation, const float& radius, const Math::vec4& color, const RenderMode& renderModes, const float& lineWidth)
{
	CircleCommand* cmd = new CircleCommand();
	cmd->shape = DebugShape::CIRCLE;

	Math::mat4 transform = Math::mat4::multiply(Math::mat4::scaling(Math::point(radius, 1.0f, radius)), Math::mat4::rotationquaternion(rotation));
	transform.translate(position);

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);

}

void DebugRenderer::DrawCircle(const Math::mat4& transform,
						const Math::vec4& color,
						const RenderMode& renderModes,
						const float& lineWidth)
{
	CircleCommand* cmd = new CircleCommand();
	cmd->shape = DebugShape::CIRCLE;

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);

}

	void DebugRenderer::DrawMesh(std::shared_ptr<Render::MeshResource> mesh,
							 const Math::mat4& transform, 
							 const Math::vec4& color,
							 const RenderMode& renderModes,
							 int primitiveGroup,
							 float lineWidth)
{
	MeshCommand* cmd = new MeshCommand();
	cmd->shape = DebugShape::MESH;

	cmd->mesh = mesh;
	cmd->primitiveGroup = primitiveGroup;

	cmd->transform = transform;

	cmd->linewidth = lineWidth;
	cmd->color = color;
	//Always draw bboxes with wireframe
	cmd->rendermode = renderModes;

	this->commandQueue.push(cmd);
}


void DebugRenderer::DrawCommands()
{
    while (!this->commandQueue.empty())
    {
        RenderCommand* currentCommand = this->commandQueue.front();
        this->commandQueue.pop();

        switch(currentCommand->shape)
        {
            case DebugShape::LINE:
            {
                this->line.Draw(currentCommand);
                break;
            }
			case DebugShape::BOX:
			{
				this->box.Draw(currentCommand);
				break;
			}
			case DebugShape::MESH:
			{
				this->mesh.Draw(currentCommand);
				break;
			}
			case DebugShape::CONE:
	        {
				this->cone.Draw(currentCommand);
				break;
	        }
			case DebugShape::CIRCLE:
	        {
				this->circle.Draw(currentCommand);
				break;
	        }
            default:
            {
                printf("Debug::RenderShape not fully implemented!\n");
                assert(false);
                break;
            }

        }

        delete currentCommand;
    }

    return;
}


}
