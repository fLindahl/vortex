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


		GLuint& GetShadowMap() { return this->buffer; }
	private:
		//Uniform Buffer Object
		GLuint ubo[1];
		GLuint shadowmap;
		GLuint shadowmapFBO;
		GLuint shadowmapprogram;
		GLuint sendtothisshaderprogram;

		struct  shadowUniformBuffer
		{
			Math::mat4 LSM;
			Math::mat4 lProj;
		}shadUniformBuffer;

	

	};


}
