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

private:
	LightServer* lightServer;
	//std::shared_ptr<ShaderObject> computeShader;
	GLuint lightCullingProgram;

};

}