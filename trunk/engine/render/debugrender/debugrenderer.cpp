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
    this->line.shaderProgram = Render::ShaderServer::Instance()->LoadShader("outline")->GetProgram();
    return;
}

void DebugRenderer::DrawLine(const Math::point &startPoint,
                             const Math::point &endPoint,
                             const float &lineWidth,
                             const Math::vec4 &startColor,
                             const Math::vec4 &endColor)
{
    LineCommand* cmd = new LineCommand();
    cmd->shape = DebugShape::LINE;
    cmd->startpoint = startPoint;
    cmd->endpoint = endPoint;
    cmd->width = lineWidth;
    cmd->startcolor = startColor;
    cmd->endcolor = endColor;

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
                glUseProgram(this->line.shaderProgram);
                //glPolygonMode(GL_FRONT, GL_LINE);
                this->line.Draw(currentCommand);
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

    glPolygonMode(GL_FRONT, GL_FILL);

    return;
}


}