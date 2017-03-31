#pragma once
#include "framepass.h"
#include "render/server/lightserver.h"

namespace Render
{
class LightCullingPass : public FramePass
{
__DeclareClass(LightCullingPass);
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
        //GLuint tileLights;
	} uniformBlock;

	GLuint ubo[1];

	LightServer* lightServer;
	//Ptr<ShaderObject> computeShader;
	GLuint lightCullingProgram;

	GLuint buffer;
};

}