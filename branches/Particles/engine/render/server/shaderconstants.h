#pragma once

namespace Render 
{

	struct RenderState
	{
		//GL_CULL_FACE
		GLboolean cullface = GL_FALSE;
		GLenum frontface = GL_CW;
		GLenum cullmode = GL_BACK;

		//GL_BLEND 
		GLboolean blend = GL_FALSE;
		GLenum blendsourcefunc = GL_ONE;
		GLenum blenddestinationfunc = GL_ZERO;

		//GL_ALPHA_TEST
		GLboolean alphatest = GL_FALSE;
		GLenum alphafunc = GL_ALWAYS;
		GLclampf alphaclamp = 0.0f;

		//GL_DEPTH_TEST
		GLboolean depthtest = GL_FALSE;
		GLenum depthfunc = GL_LESS;
	};

    /*
     * These headers will be attached to every shader that the shaderserver compiles.
     */
	const char ShaderHeader[] =
		"#version 430\n"
		"layout (std140, binding = 0) uniform GlobalBlock\n"
		"{\n"
		"#define TILE_SIZE 16\n"
		"mat4 View;\n"
		"mat4 Projection;\n"
		"mat4 ViewProjection;\n"
		"mat4 InvView;\n"
		"mat4 InvProjection;\n"
		"mat4 InvViewProjection;\n"
		"vec4 CameraPosition;\n"
		"ivec2 ScreenSize;\n"
		"vec2 TimeAndRandom;\n"
		"uvec2 LightTileWorkGroups;\n"
		"};\n"
		"\n";

	const char VertexShaderHeader[] =
		"uniform mat4 Model;\n";

    const char FragmentShaderHeader[] =
        "";
}