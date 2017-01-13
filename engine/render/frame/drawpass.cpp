#include "config.h"
#include "render/server/frameserver.h"
#include "drawpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"

namespace Render
{
DrawPass::DrawPass()
{
    this->frameBufferObject = 0;
    this->buffer = 0;
}

DrawPass::~DrawPass()
{

}

void DrawPass::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferObject);
}

void DrawPass::Execute()
{
    GLuint currentProgram = 0;

    for (Material* material : this->materials)
    {
        auto shader = material->GetShader(this->name);

        if (shader->GetProgram() != currentProgram)
        {
            currentProgram = shader->GetProgram();
            glUseProgram(currentProgram);
        }

        //TODO: Renderstates?

        //TODO: Per surface
        for (auto surface : material->SurfaceList())
        {
            for (index_t i = 0; i < surface->TextureList().Size(); i++)
            {
                surface->TextureList()[i]->BindTexture(i); //TODO: slot?
            }

            for (index_t i = 0; i < surface->ParameterList().Size(); i++)
            {
                //TODO: Move this elsewhere
                switch (surface->ParameterList()[i]->var.GetType())
                {
                    case Util::Variable::Type::Float:
                        shader->setUni1f(*surface->ParameterList()[i]->var.GetFloat(), surface->ParameterList()[i]->name);
                        break;

                    case Util::Variable::Type::Vector4:
                        shader->setUniVector4fv(surface->ParameterList()[i]->var.GetVector4(), surface->ParameterList()[i]->name);
                        break;

                    default:
                        printf("ERROR : Parameter might not be fully implemented! \n");
                        assert(false);
                        break;
                }
            }

            for (auto modelNode : surface->GetModelNodes())
            {
                //Bind mesh
                //TODO: We should probably check and make sure we don't bind these more than once
                modelNode->modelInstance->GetMesh()->Bind();

                for (GraphicsProperty* graphicsProperty : modelNode->modelInstance->GetGraphicsProperties())
                {
                    shader->setModelMatrix(graphicsProperty->getModelMatrix());
                    modelNode->modelInstance->GetMesh()->Draw(modelNode->primitiveGroup);
                }

                modelNode->modelInstance->GetMesh()->Unbind();
            }
        }
    }

    FramePass::Execute();
}
void DrawPass::Setup()
{


    FramePass::Setup();
}

}