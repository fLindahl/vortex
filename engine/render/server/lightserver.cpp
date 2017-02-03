#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"

namespace Render
{

LightServer::LightServer() : pointLightBuffer(0), spotLightBuffer(0), visiblePointLightIndicesBuffer(0), visibleSpotLightIndicesBuffer(0), workGroupsX(0), workGroupsY(0)
{
	// Generate our shader storage buffers
	glGenBuffers(1, &pointLightBuffer);
	glGenBuffers(1, &spotLightBuffer);
	glGenBuffers(1, &visiblePointLightIndicesBuffer);
	glGenBuffers(1, &visibleSpotLightIndicesBuffer);
/*
	glGenTextures(1, &this->spotLightBuffer);
	glBindTexture(GL_TEXTURE_2D, this->spotLightBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, RenderDevice::Instance()->GetRenderResolution().x, RenderDevice::Instance()->GetRenderResolution().y, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	this->UpdatePointLightBuffer();
}

void LightServer::AddSpotLight(SpotLight& sLight)
{
	this->spotLights.Append(sLight);
	this->UpdateSpotLightBuffer();
}

void LightServer::UpdateWorkGroups()
{
	// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
	workGroupsX = (RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % 16)) / 16;
	workGroupsY = (RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % 16)) / 16;

	size_t numberOfTiles = workGroupsX * workGroupsY;

	// Bind visible Point light indices buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visiblePointLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * 512, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleSpotLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * 512, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdatePointLightBuffer()
{
	size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numPointLights = this->pointLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPointLights * sizeof(PointLight), &this->pointLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdateSpotLightBuffer()
{
	size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numSpotLights = this->spotLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, spotLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numSpotLights * sizeof(SpotLight), &this->spotLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

}