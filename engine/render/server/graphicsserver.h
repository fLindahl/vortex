/**
Singleton GraphicsServer

Contains all graphical objects and is used to call their draw functions and more.
*/

#pragma once
#include "render/properties/graphicsproperty.h"
#include <unordered_map>
#include "GLFW/glfw3.h"

#include "foundation/util/array.h"

namespace Render
{

class ShaderObject;

class GraphicsServer
{
private:
	GraphicsServer()
	{
		glGenBuffers(1, this->ubo);
	}

	struct UniformBufferBlock
	{
		Math::Matrix4 ViewMatrix;
		Math::Matrix4 ProjectionMatrix;
	} uniformBufferBlock;

	//Uniform Buffer Object
	GLuint ubo[1];

public:
	static GraphicsServer* Instance()
	{
		static GraphicsServer instance;
		return &instance;
	}

	// C++ 11
	// Delete the methods we don't want.
	GraphicsServer(GraphicsServer const&) = delete;
	void operator=(GraphicsServer const&) = delete;

	void addShaderObject(ShaderObject* obj) { this->shaderObjects.Append(obj); }

	//This renders all graphicsproperties.
	void Render();

private:
	//contains all the shader objects that we've loaded
	Util::Array<ShaderObject*> shaderObjects;
};

}