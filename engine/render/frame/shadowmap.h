#pragma once
#include "drawpass.h"
#include "foundation/math/matrix4.h"
#include "render/server/lightserver.h"

namespace Render
{
	class ShadowMap : public DrawPass
	{
	public:
		ShadowMap();
		~ShadowMap();

		void Setup();
		void Execute();

		void UpdateResolution();


		GLuint GetShadowMapFrameBuffer() { return this->frameBufferObject; }
	private:
		//Uniform Buffer Object
		GLuint ubo[1];
		GLuint ubo2[1];

		struct  shadowUniformBuffer
		{
			Math::mat4 DepthBias;
			Math::mat4 lightM;
			Math::mat4 lightV;
		}shadUniformBuffer;

		struct SpotLightBuffer
		{
			LightServer::SpotLight spotLight;
		}spotLightBuffer;

	};


}
