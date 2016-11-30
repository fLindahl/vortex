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
#include "frameserver.h"
#include "render/resources/material.h"
#include "lightserver.h"
#include "render/resources/framepass.h"

namespace Render
{
RenderDevice::RenderDevice()
{
	glGenBuffers(1, this->ubo);
	currentProgram = 0;

	// Generate our shader storage buffers
	glGenBuffers(1, &lightBuffer);
	glGenBuffers(1, &visibleLightIndicesBuffer);
}

void RenderDevice::Initialize()
{

	
#ifdef _LIGHT_DEBUG
	this->lightDebugShader = std::make_shared<ShaderObject>();

	// Set name
	this->lightDebugShader->SetName("lightShader");

	GLuint vertShader = ShaderServer::Instance()->LoadVertexShader("resources/shaders/vertex/static_obj.vert");
	GLuint fragmentShader = ShaderServer::Instance()->LoadFragmentShader("resources/shaders/fragment/light_debug.frag");

	this->lightDebugShader->setVertexShader(vertShader);
	this->lightDebugShader->setFragmentShader(fragmentShader);
	this->lightDebugShader->LinkShaders();
#endif // _DEBUG

}

void RenderDevice::SetResolution(const Resolution& res)
{
	this->resolution = res;
	UpdateWorkGroups();
}

void RenderDevice::SetResolution(const int& x, const int& y)
{
	this->resolution.x = x; 
	this->resolution.y = y;
	UpdateWorkGroups();
}

void RenderDevice::UpdateWorkGroups()
{
	// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
	workGroupsX = (resolution.x + (resolution.x % 16)) / 16;
	workGroupsY = (resolution.y + (resolution.y % 16)) / 16;

	size_t numberOfTiles = workGroupsX * workGroupsY;

	// Bind visible light indices buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * 1024, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RenderDevice::UpdateLightBuffer()
{
	size_t numberOfTiles = workGroupsX * workGroupsY;

	size_t numLights = LightServer::Instance()->pointLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numLights * sizeof(PointLight), &LightServer::Instance()->pointLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RenderDevice::Render()
{
    //Set global matrix uniform buffer block for this frame
    uniformBufferBlock.View = Graphics::MainCamera::Instance()->getViewMatrix();
    uniformBufferBlock.Projection = Graphics::MainCamera::Instance()->getProjectionMatrix();
	uniformBufferBlock.ViewProjection = Math::mat4::multiply(Graphics::MainCamera::Instance()->getViewMatrix(), Graphics::MainCamera::Instance()->getProjectionMatrix());

	uniformBufferBlock.InvView = Math::mat4::inverse(uniformBufferBlock.View);
	uniformBufferBlock.InvProjection = Math::mat4::inverse(uniformBufferBlock.Projection);
	uniformBufferBlock.InvViewProjection = Math::mat4::inverse(uniformBufferBlock.ViewProjection);

	uniformBufferBlock.CameraPos = Graphics::MainCamera::Instance()->GetPosition();

	uniformBufferBlock.ScreenSize = this->resolution;
	uniformBufferBlock.TimeAndRandom[0] = (GLfloat)glfwGetTime();

	//TODO: Add some interesting random function here!
	uniformBufferBlock.TimeAndRandom[1] = 0.0f; 

	uniformBufferBlock.LightTileWorkGroups[0] = workGroupsX;
	uniformBufferBlock.LightTileWorkGroups[1] = workGroupsY;

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo[0]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo[0]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniformBufferBlock), &uniformBufferBlock, GL_STATIC_DRAW);

    //HACK: Render states should be done per shaderobject (in some cases)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//----------------
	// Start rendering

	//TODO: We should iterate through every pass instead of doing it this way!

	// Depth pre-pass
	std::weak_ptr<FramePass> d = FrameServer::Instance()->Depth;
	auto depthPass = d.lock();

	// Bind the depth map's frame buffer and draw the depth map to it
	depthPass->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	std::string& str = depthPass->name;
	
	for (Material* material : FrameServer::Instance()->Depth->materials)
	{
		if (material->GetShader(str)->GetProgram() != currentProgram)
		{
			currentProgram = material->GetShader(str)->GetProgram();
			glUseProgram(currentProgram);
		}

		//TODO: Renderstates?

		for (ModelInstance* modelInstance : material->getModelInstances())
		{
			//Bind mesh
			//TODO: We should probably check and make sure we don't bind these more than once
			modelInstance->GetMesh()->Bind();
			
			for (GraphicsProperty* graphicsProperty : modelInstance->GetGraphicsProperties())
			{
				material->GetShader(str)->setModelMatrix(graphicsProperty->getModelMatrix());
				modelInstance->GetMesh()->Draw();
			}

			modelInstance->GetMesh()->Unbind();
		}		
	}
	
	//Unbind Depth FrameBufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	// Light culling compute shader
	GLuint lightCullingProgram = FrameServer::Instance()->lightCullingProgram;
	glUseProgram(lightCullingProgram);
	
	// Bind depth map texture to texture location 4 (which will not be used by any model texture)
	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(lightCullingProgram, "depthMap"), 4);
	glUniform1i(glGetUniformLocation(lightCullingProgram, "lightCount"), LightServer::Instance()->pointLights.Size());
	glBindTexture(GL_TEXTURE_2D, depthPass->buffer);

	// Bind shader storage buffer objects for the light and indice buffers

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, visibleLightIndicesBuffer);
	
	// Dispatch the compute shader, using the workgroup values calculated earlier
	glDispatchCompute(workGroupsX, workGroupsY, 1);

	// Unbind the depth map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef _LIGHT_DEBUG

	glUseProgram(this->lightDebugShader->GetProgram());

	glUniform1i(glGetUniformLocation(this->lightDebugShader->GetProgram(), "totalLightCount"), LightServer::Instance()->pointLights.Size());

	// Render Lights only within tiles
	for (Material* material : FrameServer::Instance()->FlatGeometryLit->materials)
	{
		for (ModelInstance* modelInstance : material->getModelInstances())
		{
			//Bind mesh
			modelInstance->GetMesh()->Bind();
			for (GraphicsProperty* graphicsProperty : modelInstance->GetGraphicsProperties())
			{
				material->GetShader(str)->setModelMatrix(graphicsProperty->getModelMatrix());
				modelInstance->GetMesh()->Draw();
			}
			modelInstance->GetMesh()->Unbind();
		}
	}


#else

	// Render pass
	str = FrameServer::Instance()->FlatGeometryLit->name;
	for (Material* material : FrameServer::Instance()->FlatGeometryLit->materials)
    {
		if (material->GetShader(str)->GetProgram() != currentProgram)
		{
			currentProgram = material->GetShader(str)->GetProgram();
			glUseProgram(currentProgram);
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
				material->GetShader(str)->setUni1f(*material->ParameterList()[i]->var.GetFloat(), material->ParameterList()[i]->name);
				break;

			case Util::Variable::Type::Vector4:
				material->GetShader(str)->setUniVector4fv(material->ParameterList()[i]->var.GetVector4(), material->ParameterList()[i]->name);
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
                material->GetShader(str)->setModelMatrix(graphicsProperty->getModelMatrix());
                modelInstance->GetMesh()->Draw();
            }

            modelInstance->GetMesh()->Unbind();
        }
    }

#endif

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
}




}