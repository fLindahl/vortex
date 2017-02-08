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

std::shared_ptr<CubeMapNode> LightServer::GetClosestCubemapToPoint(const Math::point& point)
{
	float closestDistance = FLT_MAX;
	std::shared_ptr<CubeMapNode> closestCubemap = nullptr;

	float distanceSq;

	for (auto cubemap : this->cubemapNodes)
	{
		distanceSq = Math::point::distancesq(point, cubemap->GetPosition());
		if (distanceSq < closestDistance)
		{
			closestDistance = distanceSq;
			closestCubemap = cubemap;
		}
	}

	return closestCubemap;
}

void LightServer::RegenerateCubemaps()
{
	for (auto cubemap : this->cubemapNodes)
	{
		cubemap->GenerateCubeMap();
	}
}

}