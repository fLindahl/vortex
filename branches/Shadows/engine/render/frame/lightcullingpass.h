#pragma once
#include "framepass.h"
#include "render/server/lightserver.h"

namespace Render
{
class LightCullingPass : public FramePass
{
public:
	LightCullingPass();
	~LightCullingPass();

    void Setup();

    void Execute();

	GLuint GetBuffer() {return this->buffer;}

private:
	struct UniformBlock
	{
		GLint pointLightCount;
		GLint spotLightCount;
		GLint directionalLightCount;
        GLuint tileLights;
	} uniformBlock;

	GLuint ubo[1];

	LightServer* lightServer;
	//std::shared_ptr<ShaderObject> computeShader;
	GLuint lightCullingProgram;

	GLuint buffer;
};

}