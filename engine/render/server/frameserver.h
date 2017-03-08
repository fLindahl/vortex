#pragma once
#include <memory>
#include <map>
#include "GL/glew.h"
#include "foundation/util/array.h"
#include "render/frame/shadowmap.h"
#include "render/frame/reflectionpass.h"
#include "render/frame/dynamicunlitpass.h"

namespace Debug { class DebugServer; }

namespace Render
{

class FramePass;
class DepthPass;
class DrawPass;
class FlatGeometryLitPass;
class PickingPass;
class ReflectionPass;
class ShadowMap; ///SWARLEY
class ParticleComputePass;
class VSMShadowMap; // SWARLEY

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
	std::shared_ptr<PickingPass> GetPickingPass();
	std::shared_ptr<ShadowMap> GetShadowMap();
	std::shared_ptr<ParticleComputePass> GetParticleComputePass();
	std::shared_ptr<VSMShadowMap> GetVSMShadowMap();


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

	std::shared_ptr<PickingPass> pickingPass;

	/// For lit objects
	std::shared_ptr<FlatGeometryLitPass> FlatGeometryLit;

	/// For computing reflections
	std::shared_ptr<ReflectionPass> reflectionPass;

	///SWARLEY
	std::shared_ptr<ShadowMap> shadowmap;

	std::shared_ptr<VSMShadowMap> VSMshadowmap;
	
	/// For icons etc.
	/// Objects rendered in this pass are not shown in reflections
	std::shared_ptr<DynamicUnlitPass> dynamicUnlitPass;

};

}