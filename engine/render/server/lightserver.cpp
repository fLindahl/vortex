#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"

namespace Render
{

LightServer::LightServer() : pointLightBuffer(0), spotLightBuffer(0), visiblePointLightIndicesBuffer(0), visibleSpotLightIndicesBuffer(0), workGroupsX(0), workGroupsY(0), tileLights(512)
{
	// Generate our shader storage buffers
	glGenBuffers(1, &pointLightBuffer);
	glGenBuffers(1, &spotLightBuffer);
	glGenBuffers(1, &visiblePointLightIndicesBuffer);
	glGenBuffers(1, &visibleSpotLightIndicesBuffer);
}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	this->UpdatePointLightBuffer();
}

void LightServer::AddSpotLight(SpotLight& sLight)
{
	/// calculate the radius of the bottom cirlce
	sLight.radius = (float)tan(Math::Deg2Rad(sLight.angle)) * sLight.length;

	/// Get perpendicular direction
	Math::vec4 m = Math::vec4::normalize(Math::vec4::cross3(sLight.coneDirection, sLight.position));
	Math::vec4 Q1 = sLight.position + sLight.coneDirection * sLight.length - m * sLight.radius;
	/// Get perpendicular, -direction
	m = Math::vec4::normalize(Math::vec4::cross3(sLight.coneDirection * -1.0f, sLight.position));
	Math::vec4 Q2 = sLight.position + sLight.coneDirection * sLight.length - m * sLight.radius;

	/// Calculate the Mid Point of the Sphere
	float oneOverThree = 1.0f/3.0f;
	sLight.midPoint = (sLight.position + Q1 + Q2) * oneOverThree;
	sLight.midPoint.set_w(1.0f);

    /// Calculate the radius for the sphere
	sLight.fRadius = (sLight.midPoint - sLight.position).length();

	this->spotLights.Append(sLight);
	this->UpdateSpotLightBuffer();
}

void LightServer::UpdateWorkGroups()
{
	// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
	workGroupsX = (GLuint)(RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % 16)) / 16;
	workGroupsY = (GLuint)(RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % 16)) / 16;

	size_t numberOfTiles = workGroupsX * workGroupsY;

	// Bind visible Point light indices buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visiblePointLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * tileLights, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleSpotLightIndicesBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * tileLights, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdatePointLightBuffer()
{
	//size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numPointLights = this->pointLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numPointLights * sizeof(PointLight), &this->pointLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::UpdateSpotLightBuffer()
{
	//size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numSpotLights = this->spotLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, spotLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numSpotLights * sizeof(SpotLight), &this->spotLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::Update()
{
    this->UpdateWorkGroups();
    this->UpdateSpotLightBuffer();
}

SpotLight LightServer::GetSpotLightAtIndex(const int &index)
{
    if(index > this->GetNumSpotLights())
         return this->spotLights[index];
    else
        printf("Index was out side the Array! ");
}

}