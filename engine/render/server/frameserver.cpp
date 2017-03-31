#include "config.h"
#include "frameserver.h"
#include "renderdevice.h"
#include "render/frame/framepass.h"
#include "render/frame/depthpass.h"
#include "render/frame/drawpass.h"
#include "render/frame/lightcullingpass.h"
#include "render/frame/flatgeometrylitpass.h"
//#include "render/frame/pickingpass.h"
//#include "render/frame/reflectionpass.h"
//#include "render/frame/shadowmap.h"
//#include "render/frame/particlecomputepass.h"

namespace Render
{
	FrameServer::FrameServer()
	{
		
	}

	void FrameServer::SetupFramePasses()
	{
		//Setup the common frame passes. 
		//TODO: these should be moved to a loader and their own xml list like everything else.

		// depth pre-pass
		this->Depth = DepthPass::Create();
		this->Depth->name = "Depth";
		this->Depth->Setup();

		this->framePassByName.Add(this->Depth->name, this->Depth.upcast<Render::FramePass>());
		this->framePasses.Append(this->Depth.downcast<Render::FramePass>());

		//Light culling compute shader pass
		this->lightCullingPass = LightCullingPass::Create();
		this->lightCullingPass->name = "LightCulling";
		this->lightCullingPass->Setup();
		
		this->framePassByName.Add(this->lightCullingPass->name, this->lightCullingPass);
		this->framePasses.Append(this->lightCullingPass);

		//Picking pass
		//TODO: we should be able to deactivate this
		
		this->pickingPass = PickingPass::Create();
		this->pickingPass->name = "Picking";
		this->pickingPass->Setup();
		
		this->framePassByName.Add(this->pickingPass->name, this->pickingPass.downcast<FramePass>());
		
		if (RenderDevice::Instance()->GetPickingEnabled())
		{
			this->framePasses.Append(this->pickingPass.downcast<Render::FramePass>());
		}

		// FlatGeometryLit pass
		this->FlatGeometryLit = FlatGeometryLitPass::Create();
		this->FlatGeometryLit->name = "FlatGeometryLit";
		this->FlatGeometryLit->Setup();

		this->framePassByName.Add(this->FlatGeometryLit->name, this->FlatGeometryLit.downcast<Render::FramePass>());
		this->framePasses.Append(this->FlatGeometryLit.downcast<FramePass>());

		//Particle compute shader pass
		this->particleComputePass = ParticleComputePass::Create();
		this->particleComputePass->name = "ParticleCompute";
		this->particleComputePass->Setup();
		
		this->framePassByName.Add(this->particleComputePass->name, this->particleComputePass.downcast<FramePass>());
		this->framePasses.Append(this->particleComputePass.downcast<Render::FramePass>());

		// Reflection pass
		//this->reflectionPass = ReflectionPass::Create();
		//this->reflectionPass->name = "Reflection";
		//this->reflectionPass->Setup();
		//
		//this->framePassByName.Add(this->reflectionPass->name, this->reflectionPass.downcast<FramePass>());
		//this->framePasses.Append(this->reflectionPass.downcast<FramePass>());
		
		// Shadow map pass
		//this->shadowmap = ShadowMap::Create();
		//this->shadowmap->name = "ShadowMap";
		//this->shadowmap->Setup();
		//
		//this->framePassByName.Add(this->shadowmap->name, this->shadowmap.downcast<FramePass>());
		//this->framePasses.Append(this->shadowmap.downcast<FramePass>());
		
		// Dynamic unlit pass
		// Uses the regular flat geometry lit framebuffer object but renders later (after shadows and reflections)
		this->dynamicUnlitPass = DynamicUnlitPass::Create();
		this->dynamicUnlitPass->name = "DynamicUnlit";
		this->dynamicUnlitPass->Setup();
		
		this->framePassByName.Add(this->dynamicUnlitPass->name, this->dynamicUnlitPass.downcast<FramePass>());
		this->framePasses.Append(this->dynamicUnlitPass.downcast<Render::FramePass>());

		//Set final color buffer for easy access
		RenderDevice::Instance()->SetFinalColorBuffer(this->FlatGeometryLit->buffer);
	}

	void FrameServer::UpdateResolutions()
	{
		//TODO: Loop through all framepasses and update each one of their resolutions if needed.
		this->Depth->UpdateResolution();
		this->FlatGeometryLit->UpdateResolution();		
		//this->reflectionPass->UpdateResolution();
		this->pickingPass->UpdateResolution();
		//this->shadowmap->UpdateResolution();
		this->dynamicUnlitPass->UpdateResolution();
	}

	Ptr<FramePass> FrameServer::GetFramePass(const Util::String& name)
	{
		if (this->HasPassNamed(name))
			return this->framePassByName[name];
		else
		{
			printf("ERROR: No framepass named %s!\n", name.AsCharPtr());
			assert(false);
			return nullptr;
		}
	}
	
	bool FrameServer::HasPassNamed(const Util::String&nName)
	{
		return (this->framePassByName.Contains(nName));
	}

	Ptr<DepthPass> FrameServer::GetDepthPass()
	{
		return this->Depth;
	}

	Ptr<FramePass> FrameServer::GetLightCullingPass()
	{
		return this->lightCullingPass;
	}

	Ptr<FlatGeometryLitPass> FrameServer::GetFlatGeometryLitPass()
	{
		return this->FlatGeometryLit;
	}

	//Ptr<ReflectionPass> FrameServer::GetReflectionPass()
	//{
	//	return this->reflectionPass;
	//}
	
	Ptr<PickingPass> FrameServer::GetPickingPass()
	{
		return this->pickingPass;
	}
	
	
	Ptr<ParticleComputePass> FrameServer::GetParticleComputePass()
	{
		return this->particleComputePass;
	}
	
	//Ptr<Render::ShadowMap> FrameServer::GetShadowMap()
	//{
	//	return this->shadowmap;
	//}
	
}