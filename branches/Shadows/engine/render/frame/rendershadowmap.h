#pragma once
#include "drawpass.h"
#include "foundation/math/matrix4.h"
#include "render/server/lightserver.h"

namespace Render
{
	class RenderShadowMap : public DrawPass
	{
	public:
		RenderShadowMap();
		~RenderShadowMap();

		void Setup();
		void Execute();

		void UpdateResolution();

		void SetShadowMapBuffer(const GLuint& buffer) { this->shadowMapBuffer = buffer; }

	private:
		GLuint shadowMapBuffer;

		GLuint ubo[1];

		struct  shadowRUniformBuffer
		{
			Math::mat4 lightMVP;
		}shadRUniformBuffer;

	};


}
