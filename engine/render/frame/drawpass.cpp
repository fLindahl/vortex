#include "config.h"
#include "render/server/frameserver.h"
#include "drawpass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"

namespace Render
{

__ImplementClass(Render::DrawPass, 'DRPS', Render::FramePass);

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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->frameBufferObject);
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

		shader->EnableRenderState();

        for (auto surface : material->SurfaceList())
        {
            for (uint i = 0; i < surface->TextureList().Size(); i++)
            {
				surface->TextureList()[i]->BindTexture(i);
				//TODO: This is kinda bad
				if (i == 0)
					glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_ALBEDOMAP), i);
				else if (i == 1)
					glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_NORMALMAP), i);
				else if (i == 2)
					glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_SPECULARMAP), i);
				else if (i == 3)
					glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_ROUGHNESSMAP), i);
            }

            for (uint i = 0; i < surface->ParameterList().Size(); i++)
            {
                //TODO: Move this elsewhere
                switch (surface->ParameterList()[i]->var.GetType())
                {
                    case Util::Variable::Type::Float:
                        shader->setUni1f(*surface->ParameterList()[i]->var.GetFloat(), surface->ParameterList()[i]->name.AsCharPtr());
                        break;

                    case Util::Variable::Type::Vector4:
                        shader->setUniVector4fv(surface->ParameterList()[i]->var.GetVector4(), surface->ParameterList()[i]->name.AsCharPtr());
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
					shader->setInvModelMatrix(graphicsProperty->getInvModelMatrix());
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

void DrawPass::UpdateResolution()
{
	
}

}