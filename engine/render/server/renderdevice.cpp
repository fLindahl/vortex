#include "config.h"
#include "GL/glew.h"
#include "renderdevice.h"
#include "resourceserver.h"
#include "render/graphics/camera.h"
#include "render/resources/shaderobject.h"
#include "render/shadersemantics.h"
#include "render/resources/modelinstance.h"
#include "render/resources/meshresource.h"
#include "render/resources/textureresource.h"
#include "render/properties/graphicsproperty.h"
#include "foundation/util/variable.h"

namespace Render
{

RenderDevice::RenderDevice()
{
	glGenBuffers(1, this->ubo);
	currentProgram = 0;
}

void RenderDevice::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set global matrix uniform buffer block for this frame

    //TODO: Get these working!
    uniformBufferBlock.View = Graphics::MainCamera::Instance()->getViewMatrix();
    uniformBufferBlock.Projection = Graphics::MainCamera::Instance()->getProjectionMatrix();
	uniformBufferBlock.ViewProjection = Math::mat4::multiply(Graphics::MainCamera::Instance()->getViewMatrix(), Graphics::MainCamera::Instance()->getProjectionMatrix());

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);

    //HACK: Render states should be done per shaderobject (in some cases)
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    for (Material* material: materials)
    {
		if (material->GetShader()->GetProgram() != currentProgram)
		{
			glUseProgram(material->GetShader()->GetProgram());
			currentProgram = material->GetShader()->GetProgram();
		}

		for (index_t i = 0; i < material->TextureList().Size(); i++)
		{
			material->TextureList()[i]->BindTexture(i); //TODO: slot?
		}

        //TODO: Renderstates?


		//TODO: Per surface
		for (index_t i = 0; i < material->ParameterList().Size(); i++)
		{
			//TODO: Move this elsewhere
			switch (material->ParameterList()[i]->var.GetType())
			{
			case Util::Variable::Type::Float:
				material->GetShader()->setUni1f(*material->ParameterList()[i]->var.GetFloat(), material->ParameterList()[i]->name);
				break;

			case Util::Variable::Type::Vector4:
				material->GetShader()->setUniVector4fv(material->ParameterList()[i]->var.GetVector4(), material->ParameterList()[i]->name);
				break;

			default:
				printf("ERROR : Parameter might not be fully implemented! \n");
				assert(false);
				break;
			}
		}

        for (ModelInstance* modelInstance : material->getModelInstances())
        {
            //Bind mesh
            //TODO: We should probably check and make sure we don't bind these more than once
            modelInstance->GetMesh()->Bind();


            for (GraphicsProperty* graphicsProperty : modelInstance->GetGraphicsProperties())
            {
                material->GetShader()->setModelMatrix(graphicsProperty->getModelMatrix());
                modelInstance->GetMesh()->Draw();
            }

            modelInstance->GetMesh()->Unbind();
        }
    }
}

}