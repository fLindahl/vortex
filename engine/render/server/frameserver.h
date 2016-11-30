#pragma once
#include <memory>
#include <unordered_map>
#include "GL/glew.h"
#include "foundation/util/array.h"

namespace Render
{

class FramePass;

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

	std::shared_ptr<FramePass> GetFramePass(const std::string& name);
	bool HasPassNamed(const std::string& name);

private:
	const unsigned int MAX_NUM_LIGHTS = 1024;

	friend class RenderDevice;

	//Contains all Framepasses.
	//Key must be unique to each Pass. the key is the pass name
	std::unordered_map<std::string, std::shared_ptr<FramePass>> framePassByName;

	Util::Array<std::shared_ptr<FramePass>> framePasses;
	
	/// Early depth testing
	std::shared_ptr<FramePass> Depth;
	
	/// Used for lightculling as part of tiled forward rendering.
	//std::shared_ptr<ComputeShaderObject> lightCulling;
	GLuint lightCullingProgram;

	/// For lit objects
	std::shared_ptr<FramePass> FlatGeometryLit;



};

}