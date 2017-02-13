#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"
#include "render/resources/cubemapnode.h"

namespace Render
{

LightServer::LightServer() : lightBuffer(0), visibleLightIndicesBuffer(0), workGroupsX(0), workGroupsY(0)
{
	// Generate our shader storage buffers
	glGenBuffers(1, &lightBuffer);
	glGenBuffers(1, &visibleLightIndicesBuffer);
}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	this->UpdateLightBuffer();
}

void LightServer::UpdateWorkGroups()
{
	// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
	workGroupsX = (RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % 16)) / 16;
	workGroupsY = (RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % 16)) / 16;

	size_t numberOfTiles = workGroupsX * workGroupsY;

	// Bind visible light indices buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * 1024, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdateLightBuffer()
{
	size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numLights = this->pointLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numLights * sizeof(PointLight), &this->pointLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::AddCubeMap(std::shared_ptr<CubeMapNode> node)
{
	if (!this->cubemapNodes.Find(node))
	{
		this->cubemapNodes.Append(node);
	}
}

void LightServer::RemoveCubeMap(std::shared_ptr<CubeMapNode> node)
{
	auto it = this->cubemapNodes.Find(node);

	if (it != nullptr)
	{
		this->cubemapNodes.RemoveSwap(it);
	}
}

Util::Array<std::shared_ptr<CubeMapNode>>& LightServer::GetClosestCubemapToPoint(const Math::point& point)
{
	float closestDistance = FLT_MAX;
	std::shared_ptr<CubeMapNode> closestCubemap = nullptr;

	float distance;

	//Clear Array first!
	selectedInfluenceVolumes.Reset();

	for (auto cubemap : this->cubemapNodes)
	{
		
		distance = (point - cubemap->GetPosition()).length();
		
		const float& outerRange = cubemap->OuterScale().x();
		const float& innerRange = cubemap->InnerScale().x();

		if (distance < innerRange)
		{
			//Early out
		}
		
		if (distance < outerRange)
		{
			cubemap->CalculateInfluenceWeights(point);
			selectedInfluenceVolumes.Append(cubemap);
		}
		
	}

	//TODO: Maybe sort by NDF?
	//selectedInfluenceVolumes.Sort();
	CalculateBlendMapFactors();

	return this->selectedInfluenceVolumes;
}

void LightServer::RegenerateCubemaps()
{
	for (auto cubemap : this->cubemapNodes)
	{
		cubemap->GenerateCubeMap();
	}
}

void LightServer::CalculateBlendMapFactors()
{
	// First calc sum of NDF and InvDNF to normalize value
	float sumNDF = 0.0f;
	float invSumNDF = 0.0f;
	float sumBlendFactor = 0.0f;
	
	const size_t num = this->selectedInfluenceVolumes.Size();
	
	if (num > 1)
	{
		for (auto cubemap : this->selectedInfluenceVolumes)
		{
			sumNDF += cubemap->NDF;
			invSumNDF += (1.0f - cubemap->NDF);
		}

		for (auto cubemap : this->selectedInfluenceVolumes)
		{
			cubemap->blendFactor = (1.0f - (cubemap->NDF / sumNDF)) / (num - 1);
			cubemap->blendFactor *= ((1.0f - cubemap->NDF) / invSumNDF);
			sumBlendFactor += cubemap->blendFactor;
		}

		// Normalize BlendFactor
		if (sumBlendFactor == 0.0f) // Possible with custom weight
		{
			sumBlendFactor = 1.0f;
		}

		float constVal = 1.0f / sumBlendFactor;
		for (auto cubemap : this->selectedInfluenceVolumes)
		{
			cubemap->blendFactor *= constVal;
		}
	}
	else if (num > 0)
	{
		this->selectedInfluenceVolumes[0]->blendFactor = 1.0f;
	}
}

}