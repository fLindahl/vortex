#include "config.h"
#include "cubemapnode.h"

namespace Render
{

CubeMapNode::CubeMapNode() :
		IsLoaded(false),
		resolution({ 128, 128 }),
		mipLevels(6)		
{
	
}

CubeMapNode::~CubeMapNode()
{

}

void CubeMapNode::GenerateCubeMap(const Math::point& wPos)
{
	//Keep the old render resolution so that we can switch back to it later.
	Render::Resolution defaultRenderResolution = Render::RenderDevice::Instance()->GetRenderResolution();
	//Update render resolution
	Render::RenderDevice::Instance()->SetRenderResolution(this->resolution);



	this->IsLoaded = true;
}

GLuint CubeMapNode::GetCubeMap()
{
	return this->cubeSampler;
}

}