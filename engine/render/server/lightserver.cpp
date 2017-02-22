#include "config.h"
#include "lightserver.h"
#include "renderdevice.h"
#include "render/resources/cubemapnode.h"

namespace Render
{

	LightServer::LightServer() : pointLightBuffer(0), spotLightBuffer(0), directionalLightBuffer(0), visiblePointLightIndicesBuffer(0), visibleSpotLightIndicesBuffer(0), workGroupsX(0), workGroupsY(0), tileLights(512)
	{
		// Generate our shader storage buffers
		glGenBuffers(1, &pointLightBuffer);
		glGenBuffers(1, &spotLightBuffer);
        glGenBuffers(1, &directionalLightBuffer);
		glGenBuffers(1, &visiblePointLightIndicesBuffer);
		glGenBuffers(1, &visibleSpotLightIndicesBuffer);

        this->directionalLight.direction.w() = 1.0f;
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
		this->spotLights.Append(sLight);
		this->UpdateSpotLightBuffer();
	}

	LightServer::SpotLight LightServer::CalculateSpotlight(Math::point color,
		Math::point position,
		Math::vec4 direction,
		float length,
		float angle)
	{

		/// calculate the radius of the bottom circle
		float radians = Math::Deg2Rad(angle);
		float radius = (float)tan(radians) * length;

		Math::point endPoint = position + Math::vector(direction) * length;

		/// Get perpendicular direction
		Math::vector m = Math::vec4::normalize(Math::vector::orthogonal(direction)) * radius;
		Math::vec4 Q1 = endPoint - m;
		Math::vec4 Q2 = endPoint + m;

		/// Calculate the Mid Point of the Sphere
		const float oneOverThree = 1.0f / 3.0f;
		Math::vec4 centerAndRadius = (position + Q1 + Q2) * oneOverThree;
		centerAndRadius.set_w(1.0f);

		GLfloat centerOffset = (centerAndRadius - position).length();

		//TODO: This isn't quite right, but it works for now. the sphere radius is a bit too big of angle is greater than 50
		//if the angle is greater than 50 degrees we cant use center-position as our radius length
		if (angle > 50)
		{
			centerAndRadius.set_w((centerAndRadius - Q1).length());
		}
		else
		{
			centerAndRadius.set_w(centerOffset);
		}

		Math::vec4 colorAndCenterOffset = color;
		colorAndCenterOffset.set_w(centerOffset);

		SpotLight spot;
		spot.centerAndRadius = centerAndRadius;
		spot.colorAndCenterOffset = colorAndCenterOffset;

		//spot.colorAndCenterOffset.set_w(centerOffset);

		//Make sure direction is normalized
		Math::vector normalizedDir = Math::vector::normalize(direction);
		//XY direction
		spot.params.x() = normalizedDir.x();
		spot.params.y() = normalizedDir.y();
		//Z is cosine of angle and sign of direction Z component
		spot.params.z() = cos(radians) * Math::sign(normalizedDir.z());
		//W is falloff radius. TEMPORARY: Set to radius for now
		spot.params.w() = radius;


		return spot;
	}

	LightServer::SpotLight& LightServer::GetSpotLightAtIndex(const int& index)
	{
		return this->spotLights[index];
	}

	void LightServer::UpdateWorkGroups()
	{
		// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
        this->workGroupsX = (GLuint)(RenderDevice::Instance()->GetRenderResolution().x + (RenderDevice::Instance()->GetRenderResolution().x % 16)) / 16;
        this->workGroupsY = (GLuint)(RenderDevice::Instance()->GetRenderResolution().y + (RenderDevice::Instance()->GetRenderResolution().y % 16)) / 16;

		size_t numberOfTiles = this->workGroupsX * this->workGroupsY;

		// Bind visible Point light indices buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->visiblePointLightIndicesBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * this->tileLights, 0, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->visibleSpotLightIndicesBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numberOfTiles * sizeof(VisibleIndex) * this->tileLights, 0, GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void LightServer::UpdatePointLightBuffer()
	{
		//size_t numberOfTiles = workGroupsX * workGroupsY;
		size_t numPointLights = this->pointLights.Size();

		// Bind light buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->pointLightBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numPointLights * sizeof(PointLight), &this->pointLights[0], GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void LightServer::UpdateSpotLightBuffer()
	{
		//size_t numberOfTiles = workGroupsX * workGroupsY;
		size_t numSpotLights = this->spotLights.Size();

		// Bind light buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->spotLightBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, numSpotLights * sizeof(SpotLight), &this->spotLights[0], GL_STATIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

    void LightServer::UpdateDirectionalLightBuffer()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, this->directionalLightBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLight), &directionalLight, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

	LightServer::SpotLight& LightServer::CreateSpotLight(Math::point color,
		Math::point position,
		Math::vec4 direction,
		float length,
		float angle)
	{
		SpotLight sLight = CalculateSpotlight(color, position, direction, length, angle);
		this->AddSpotLight(sLight);

		return this->spotLights[this->spotLights.Size() - 1];
	}

    void LightServer::AddDirectionalLight(Math::vec4 color, Math::vec4 direction)
    {
        this->directionalLight.color = color;
        this->directionalLight.direction = direction;
        this->directionalLight.direction.w() = 0.0f;

        this->UpdateDirectionalLightBuffer();
    }

	void LightServer::RemoveSpotLight(SpotLight* light)
	{
		this->spotLights.EraseSwap(light);
	}

	LightServer::PointLight& LightServer::CreatePointLight(Math::point color, Math::point position, float radius)
	{
		PointLight pLight;
		pLight.position = position;
		pLight.color = color;
		pLight.radiusAndPadding.set_x(radius);
		LightServer::Instance()->AddPointLight(pLight);
		return this->pointLights[this->pointLights.Size() - 1];
	}

	void LightServer::RemovePointLight(PointLight* light)
	{
		this->pointLights.EraseSwap(light);
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