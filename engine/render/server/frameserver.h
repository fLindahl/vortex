#pragma once
#include "core/singleton.h"
#include <memory>
#include <map>
#include "GL/glew.h"
#include "foundation/util/array.h"
#include "render/frame/depthpass.h"
#include "render/frame/drawpass.h"
#include "render/frame/framepass.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/frame/particlecomputepass.h"
#include "render/frame/shadowmap.h"
#include "render/frame/reflectionpass.h"
#include "render/frame/pickingpass.h"
#include "render/frame/dynamicunlitpass.h"

namespace Debug { class DebugServer; }

namespace Render
{

class FrameServer
{
	__DeclareSingleton(FrameServer)
public:
	///Temporary. Sets up the most common framepasses such as depthbuffer, lightculling and more.
	void SetupFramePasses();

	void UpdateResolutions();

	Ptr<FramePass> GetFramePass(const Util::String& name);

	bool HasPassNamed(const Util::String& name);

	Ptr<DepthPass> GetDepthPass();
	Ptr<FramePass> GetLightCullingPass();
	Ptr<FlatGeometryLitPass> GetFlatGeometryLitPass();
	//Ptr<ReflectionPass> GetReflectionPass();
	Ptr<PickingPass> GetPickingPass();
	//Ptr<ShadowMap> GetShadowMap();
	Ptr<ParticleComputePass> GetParticleComputePass();


private:
	friend class RenderDevice;
	friend Debug::DebugServer;
	
	const unsigned int MAX_NUM_LIGHTS = 1024;

	//Contains all Framepasses.
	//Key must be unique to each Pass. the key is the pass name
	Util::Dictionary<Util::String, Ptr<FramePass>> framePassByName;

	Util::Array<Ptr<FramePass>> framePasses;
	
	/// Early depth testing
	Ptr<DepthPass> Depth;
	
	/// Used for lightculling as part of tiled forward rendering.
	Ptr<FramePass> lightCullingPass;

	/// For lit objects
	Ptr<FlatGeometryLitPass> FlatGeometryLit;

	//Used for particle computing
	Ptr<ParticleComputePass> particleComputePass;
	
	
	Ptr<PickingPass> pickingPass;
	
	/// For computing reflections
	//Ptr<ReflectionPass> reflectionPass;
	
	/// Shadows
	//Ptr<ShadowMap> shadowmap;
	
	/// For icons etc.
	/// Objects rendered in this pass are not shown in reflections
	 Ptr<DynamicUnlitPass> dynamicUnlitPass;

};

}