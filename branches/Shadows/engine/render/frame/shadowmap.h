#pragma once
#include "drawpass.h"
#include "foundation/math/matrix4.h"

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

		struct  shadowUniformBuffer
		{
			Math::mat4 DepthBias;
			Math::mat4 lightSpaceMatrix;
		}shadUniformBuffer;

	};


}
