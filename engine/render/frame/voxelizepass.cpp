#include "config.h"
#include "render/server/frameserver.h"
#include "voxelizepass.h"
#include "render/resources/material.h"
#include "render/resources/surface.h"
#include "render/resources/meshresource.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/renderdevice.h"
#include "foundation/math/math.h"
#include "render/server/resourceserver.h"

namespace Render
{

__ImplementClass(Render::VoxelizePass, 'VXLZ', Render::DrawPass);

VoxelizePass::VoxelizePass() :
	voxelTreeSize(256)
{

}

VoxelizePass::~VoxelizePass()
{

}

void VoxelizePass::Setup()
{
	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glGenTextures(1, &this->voxelBuffer);
	glBindTexture(GL_TEXTURE_3D, this->voxelBuffer);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, this->voxelTreeSize, this->voxelTreeSize, this->voxelTreeSize, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_3D, 0);
	
	glGenBuffers(1, this->ubo);

	// Create a query object.
	glGenQueries(1, queries);

	FramePass::Setup();
}

void VoxelizePass::Execute()
{
	double time = glfwGetTime();

	GLuint64 timeElapsed = 0;
	// Query current timestamp
	glBeginQuery(GL_TIME_ELAPSED, queries[0]);
	
	//Set up for voxelization
	//Since we rely on image access instead of the standard ROP path to the framebuffer to write data into
	//our voxel grid, all framebuffer operations are disabled, including depth writes, depth testing and color writes. 
	glViewport(0, 0, this->voxelTreeSize, this->voxelTreeSize);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	GLuint currentProgram = 0;

	for (Material* material : this->materials)
	{
		auto shader = material->GetShader(this->name);

		if (shader->GetProgram() != currentProgram)
		{
			currentProgram = shader->GetProgram();
			glUseProgram(currentProgram);
		}

		const GLint location = glGetUniformLocation(currentProgram, "VoxelTree");
		if (location == -1) 
		{
			_error("Could not locate uniform location for texture in VoxelizePass::Execute()");
		}
		glUniform1i(location, 0);
		glBindImageTexture(0, this->voxelBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		for (auto surface : material->SurfaceList())
		{
			for (uint i = 0; i < surface->TextureList().Size(); i++)
			{
				surface->TextureList()[i]->BindTexture(i);
				//TODO: This is kinda bad
				if (i == 0)
					glUniform1i(glGetUniformLocation(currentProgram, VORTEX_SEMANTIC_ALBEDOMAP), i);
			}

			for (auto modelNode : surface->GetModelNodes())
			{
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

	
	//Restore GL states
	auto res = RenderDevice::Instance()->GetRenderResolution();
	glViewport(0, 0, res.x, res.y);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glEndQuery(GL_TIME_ELAPSED);
	// See how much time the rendering of object i took in nanoseconds.
	glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &timeElapsed);

	_printf("Elapsed time for voxelization: %f", (float)timeElapsed * 0.000001f);

	FramePass::Execute();
}

void VoxelizePass::UpdateResolution()
{
	const Resolution& newRes = RenderDevice::Instance()->GetRenderResolution();

	glBindTexture(GL_TEXTURE_2D, this->voxelBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, newRes.x, newRes.y, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool VoxelizePass::SetVoxelTreeSize(uint size)
{
	if (size != 0 && (size & (size - 1)) == 0)
	{
		_printf("Voxel Tree size set to %i", size);
		this->voxelTreeSize = size;
		glBindTexture(GL_TEXTURE_3D, this->voxelBuffer);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, this->voxelTreeSize, this->voxelTreeSize, this->voxelTreeSize, 0, GL_RGBA, GL_FLOAT, NULL);
		return true;
	}
	else
	{
		_warning("Voxel Tree size must be a power of 2!");
		return false;
	}
}

uint VoxelizePass::GetVoxelTreeSize() const
{
	return this->voxelTreeSize;
}


}