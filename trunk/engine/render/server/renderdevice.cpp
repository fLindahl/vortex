#include "config.h"
#include "GL/glew.h"
#include "renderdevice.h"
#include "render/graphics/camera.h"
#include "render/resources/shaderobject.h"
#include "render/shadersemantics.h"
#include "render/resources/modelinstance.h"
#include "render/resources/meshresource.h"
#include "render/resources/textureresource.h"
#include "render/properties/graphicsproperty.h"

namespace Render
{
void RenderDevice::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set global matrix uniform buffer block for this frame

    //TODO: Get these working!
    uniformBufferBlock.View = Graphics::MainCamera::Instance()->getViewMatrix().transpose();
    uniformBufferBlock.Projection = Graphics::MainCamera::Instance()->getProjectionMatrix().transpose();

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);

    //HACK: Render states should be done per shaderobject (in some cases)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    for (ShaderObject* shaderObject : shaderObjects)
    {
        shaderObject->UseProgram();

        //TODO: Renderstates?

        for (ModelInstance* modelInstance : shaderObject->getModelInstances())
        {
            //Bind mesh
            //TODO: We should probably check and make sure we don't bind these more than once
            modelInstance->getMesh()->Bind();
            modelInstance->getTexture()->BindTexture(0); //TODO: slot?

            for (GraphicsProperty* graphicsProperty : modelInstance->getGraphicsProperties())
            {
                shaderObject->setModelMatrix(graphicsProperty->getModelMatrix());
                modelInstance->getMesh()->Draw();
            }

            modelInstance->getMesh()->Unbind();
        }
    }
}




}