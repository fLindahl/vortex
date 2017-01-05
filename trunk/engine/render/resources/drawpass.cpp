#include "config.h"
#include "render/server/frameserver.h"
#include "drawpass.h"
#include "material.h"
#include "surface.h"
#include "meshresource.h"
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

            for (ModelInstance* modelInstance : surface->getModelInstances())
            {
                //Bind mesh
                //TODO: We should probably check and make sure we don't bind these more than once
                modelInstance->GetMesh()->Bind();


                for (GraphicsProperty* graphicsProperty : modelInstance->GetGraphicsProperties())
                {
                    shader->setModelMatrix(graphicsProperty->getModelMatrix());

                    //HACK: This is disgusting
                    if (graphicsProperty->outline)
                    {
                        glClearStencil(0);
                        glClear(GL_STENCIL_BUFFER_BIT);

                        // Render the mesh into the stencil buffer.
                        glEnable(GL_STENCIL_TEST);
                        glStencilFunc(GL_ALWAYS, 1, -1);
                        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

                        modelInstance->GetMesh()->Draw();

                        // Render the thick wireframe version.
                        auto p = ShaderServer::Instance()->LoadShader("outline");
                        glUseProgram(p->GetProgram());

                        p->setModelMatrix(graphicsProperty->getModelMatrix());

                        glStencilFunc(GL_NOTEQUAL, 1, -1);
                        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

                        glLineWidth(3.0f);
                        glPolygonMode(GL_FRONT, GL_LINE);

                        modelInstance->GetMesh()->Draw();

                        glPolygonMode(GL_FRONT, GL_FILL);
                        glDisable(GL_STENCIL_TEST);

                        glUseProgram(currentProgram);
                    }
                    else
                    {
                        modelInstance->GetMesh()->Draw();
                    }
                }

                modelInstance->GetMesh()->Unbind();
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