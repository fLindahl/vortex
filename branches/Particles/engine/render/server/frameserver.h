#pragma once
#include <memory>
#include <map>
#include "GL/glew.h"
#include "foundation/util/array.h"

namespace Debug { class DebugServer; }

namespace Render
{

class FramePass;
class DepthPass;
class DrawPass;
class FlatGeometryLitPass;
class LightDebugPass;

class FrameServer
{
private:
	FrameServer();

public:
	static FrameServer* Instance()
	{
		static FrameServer instance;
		return &instance;
	}

	FrameServer(FrameServer const&) = delete;
	void operator=(FrameServer const&) = delete;

	///Temporary. Sets up the most common framepasses such as depthbuffer, lightculling and more.
	void SetupFramePasses();

	void UpdateResolutions();

	std::shared_ptr<FramePass> GetFramePass(const std::string& name);

	bool HasPassNamed(const std::string& name);

	std::shared_ptr<DrawPass> GetDepthPass();
	std::shared_ptr<FramePass> GetLightCullingPass();
	std::shared_ptr<FlatGeometryLitPass> GetFlatGeometryLitPass();

private:
	friend class RenderDevice;
	friend Debug::DebugServer;
	
	const unsigned int MAX_NUM_LIGHTS = 1024;

	//Contains all Framepasses.
	//Key must be unique to each Pass. the key is the pass name
	std::map<std::string, std::shared_ptr<FramePass>> framePassByName;

	Util::Array<std::shared_ptr<FramePass>> framePasses;
	
	/// Early depth testing
	std::shared_ptr<DrawPass> Depth;
	
	/// Used for lightculling as part of tiled forward rendering.
	std::shared_ptr<FramePass> lightCullingPass;
	//GLuint lightCullingProgram;

	std::shared_ptr<LightDebugPass> lightdebugpass;

	/// For lit objects
	std::shared_ptr<FlatGeometryLitPass> FlatGeometryLit;





};

}