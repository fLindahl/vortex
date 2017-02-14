#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"
#include "render/resources/cubemapnode.h"

namespace Render
{

LightServer::LightServer() : pointLightBuffer(0), spotLightBuffer(0), directionalLightBuffer(0),
							 visiblePointLightIndicesBuffer(0), visibleSpotLightIndicesBuffer(0), visibleDirectionaltLightIndicesBuffer(0),
							 workGroupsX(0), workGroupsY(0), tileLights(512)
{
	// Generate our shader storage buffers
	glGenBuffers(1, &pointLightBuffer);
	glGenBuffers(1, &spotLightBuffer);
	glGenBuffers(1, &directionalLightBuffer);
	glGenBuffers(1, &visiblePointLightIndicesBuffer);
	glGenBuffers(1, &visibleSpotLightIndicesBuffer);
	glGenBuffers(1, &visibleDirectionaltLightIndicesBuffer);

    this->oneOverThree = 1.0f/3.0f;
}

void LightServer::AddPointLight(const PointLight& pLight)
{
	this->pointLights.Append(pLight);
	this->UpdatePointLightBuffer();
}

LightServer::PointLight& LightServer::GetPointLightAtIndex(const int& index)
{
	return this->pointLights[index];
}

void LightServer::AddSpotLight(SpotLight& sLight)
{
    this->CalculateSpotlight(sLight);
	this->spotLights.Append(sLight);
	this->UpdateSpotLightBuffer();
}

void LightServer::CalculateSpotlight(SpotLight& sLight)
{
	/// calculate the radius of the bottom cirlce
	float radius = (float)tan(Math::Deg2Rad(sLight.angle)) * sLight.length;

	/// Get perpendicular direction
	Math::vec4 m = Math::vec4::normalize(Math::vec4::cross3(sLight.coneDirection, sLight.position));
	Math::vec4 Q1 = sLight.position + sLight.coneDirection * sLight.length - m * radius;
	/// Get perpendicular, -direction
	m = Math::vec4::normalize(Math::vec4::cross3(sLight.coneDirection * -1.0f, sLight.position));
	Math::vec4 Q2 = sLight.position + sLight.coneDirection * sLight.length - m * radius;

	/// Calculate the Mid Point of the Sphere
	sLight.midPoint = (sLight.position + Q1 + Q2) * this->oneOverThree;
	sLight.midPoint.set_w(1.0f);

	/// Calculate the radius for the sphere
	sLight.fRadius = (sLight.midPoint - sLight.position).length() + radius;
}

LightServer::SpotLight& LightServer::GetSpotLightAtIndex(const int& index)
{
	return this->spotLights[index];
}

void LightServer::AddDirectionalLight(const DirectionalLight& dLight)
{
	this->directionlLights.Append(dLight);
	this->UpdateDirectionalLightBuffer();
}

LightServer::DirectionalLight& LightServer::GetDirectionalLightAtIndex(const int& index)
{
	return this->directionlLights[index];
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

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, visibleDirectionaltLightIndicesBuffer);
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

void LightServer::UpdateDirectionalLightBuffer()
{
	//size_t numberOfTiles = workGroupsX * workGroupsY;
	size_t numDirectionalLights = this->directionlLights.Size();

	// Bind light buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, directionalLightBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numDirectionalLights * sizeof(DirectionalLight), &this->directionlLights[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void LightServer::CreateSpotLight(Math::point color,
								  Math::point position,
								  Math::vec4 direction,
								  float length,
								  float angle)
{
	SpotLight sLight = SpotLight();
	sLight.color = color;
	sLight.position = position;
	sLight.coneDirection = direction;
	sLight.length = length;
	sLight.angle = angle;
	this->AddSpotLight(sLight);
}

void LightServer::CreatePointLight(Math::point color, Math::point position, float radius)
{
	PointLight pLight;
	pLight.position = position;
	pLight.color = color;
	pLight.radiusAndPadding.set_x(radius);
	this->AddPointLight(pLight);
}

void LightServer::CreateDirectionalLight(Math::point color, Math::point direction)
{
	DirectionalLight dLight;
	dLight.direction = direction;
    dLight.direction.w() = 0.0f;
	dLight.color = color;
	this->AddDirectionalLight(dLight);
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