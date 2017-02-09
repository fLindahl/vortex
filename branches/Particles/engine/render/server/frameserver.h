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
class ReflectionPass;
class ParticleComputePass;

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

	std::shared_ptr<DepthPass> GetDepthPass();
	std::shared_ptr<FramePass> GetLightCullingPass();
	std::shared_ptr<FlatGeometryLitPass> GetFlatGeometryLitPass();
	std::shared_ptr<ReflectionPass> GetReflectionPass();
	std::shared_ptr<ParticleComputePass> GetParticleComputePass();

private:
	friend class RenderDevice;
	friend Debug::DebugServer;
	
	const unsigned int MAX_NUM_LIGHTS = 1024;

	//Contains all Framepasses.
	//Key must be unique to each Pass. the key is the pass name
	std::map<std::string, std::shared_ptr<FramePass>> framePassByName;

	Util::Array<std::shared_ptr<FramePass>> framePasses;
	
	/// Early depth testing
	std::shared_ptr<DepthPass> Depth;
	
	/// Used for lightculling as part of tiled forward rendering.
	std::shared_ptr<FramePass> lightCullingPass;

	//Used for particle computing
	std::shared_ptr<ParticleComputePass> particleComputePass;

	//GLuint lightCullingProgram;

	std::shared_ptr<LightDebugPass> lightdebugpass;

	/// For lit objects
	std::shared_ptr<FlatGeometryLitPass> FlatGeometryLit;

	/// For computing reflections
	std::shared_ptr<ReflectionPass> ReflectionPass;





};

}