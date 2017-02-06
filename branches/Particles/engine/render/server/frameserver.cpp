#include "config.h"
#include "frameserver.h"
#include "renderdevice.h"
#include "render/frame/framepass.h"
#include "render/frame/depthpass.h"
#include "render/frame/drawpass.h"
#include "render/frame/lightcullingpass.h"
#include "render/frame/flatgeometrylitpass.h"
#include "render/frame/lightdebugpass.h"
#include "render/frame/reflectionpass.h"
#include "render/frame/particlecomputepass.h"

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
		this->Depth = std::make_shared<DepthPass>();
		this->Depth->name = "Depth";
		this->Depth->Setup();

		this->framePassByName.insert(std::make_pair(this->Depth->name, this->Depth));
		this->framePasses.Append(this->Depth);
		

		//Light culling compute shader pass
		this->lightCullingPass = std::make_shared<LightCullingPass>();
		this->lightCullingPass->name = "LightCulling";
		this->lightCullingPass->Setup();
		
		this->framePassByName.insert(std::make_pair(this->lightCullingPass->name, this->lightCullingPass));
		this->framePasses.Append(this->lightCullingPass);

#ifdef _DEBUG
		this->lightdebugpass = std::make_shared<LightDebugPass>();
		this->lightdebugpass->Setup();
#endif

		// FlatGeometryLit pass
		this->FlatGeometryLit = std::make_shared<FlatGeometryLitPass>();
		this->FlatGeometryLit->name = "FlatGeometryLit";
		this->FlatGeometryLit->Setup();

		this->framePassByName.insert(std::make_pair(this->FlatGeometryLit->name, this->FlatGeometryLit));
		this->framePasses.Append(this->FlatGeometryLit);

		//Particle compute shader pass
		this->particleComputePass = std::make_shared<ParticleComputePass>();
		this->particleComputePass->name = "ParticleCompute";
		this->particleComputePass->Setup();

		this->framePassByName.insert(std::make_pair(this->particleComputePass->name, this->particleComputePass));
		this->framePasses.Append(this->particleComputePass);

		// FlatGeometryLit pass
		this->ReflectionPass = std::make_shared<Render::ReflectionPass>();
		this->ReflectionPass->name = "Reflection";
		this->ReflectionPass->Setup();

		this->framePassByName.insert(std::make_pair(this->ReflectionPass->name, this->ReflectionPass));
		this->framePasses.Append(this->ReflectionPass);

		//Set final color buffer for easy access
		RenderDevice::Instance()->SetFinalColorBuffer(this->FlatGeometryLit->buffer);

	}

	void FrameServer::UpdateResolutions()
	{
		//TODO: Loop through all framepasses and update each one of their resolutions if needed.
		this->Depth->UpdateResolution();
		this->FlatGeometryLit->UpdateResolution();		
		this->ReflectionPass->UpdateResolution();
	}

	std::shared_ptr<FramePass> FrameServer::GetFramePass(const std::string& name)
	{
		if (this->HasPassNamed(name))
			return this->framePassByName[name];
		else
		{
			printf("ERROR: No framepass named %s!\n", name);
			assert(false);
			return nullptr;
		}
	}
	
	bool FrameServer::HasPassNamed(const std::string &nName)
	{
		return (this->framePassByName.count(nName) > 0);
	}

	std::shared_ptr<DepthPass> FrameServer::GetDepthPass()
	{
		return this->Depth;
	}

	std::shared_ptr<FramePass> FrameServer::GetLightCullingPass()
	{
		return this->lightCullingPass;
	}

	std::shared_ptr<FlatGeometryLitPass> FrameServer::GetFlatGeometryLitPass()
	{
		return this->FlatGeometryLit;
	}

	std::shared_ptr<ReflectionPass> FrameServer::GetReflectionPass()
	{
		return this->ReflectionPass;
	}

	std::shared_ptr<ParticleComputePass> FrameServer::GetParticleComputePass()
	{
		return this->particleComputePass;
	}
}
