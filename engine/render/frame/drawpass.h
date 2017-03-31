//-----------------
/*
Class: Render::DrawPass

A base class for all passes that need to render something to screen or to a framebuffer.

-------*/

#pragma once
#include "framepass.h"

namespace Render
{

class DrawPass : public FramePass
{

__DeclareClass(DrawPass)

public:
    DrawPass();
    ~DrawPass();

	/// get OGL Framebuffer object
	GLuint& GetFrameBufferObject() { return this->frameBufferObject; }

	/// Returns this framebuffer objects render texture
	GLuint& GetBuffer() { return this->buffer; }

	void AddMaterial(Material* mat) { this->materials.Append(mat); }

	void BindFrameBuffer();

	GLuint GetTexture() { return this->buffer; }

    virtual void Setup();

    virtual void Execute();

	virtual void UpdateResolution();

protected:
	friend class RenderDevice;
	friend class FrameServer;

	/// Frame Buffer Object
	GLuint frameBufferObject;

	/// Texture
	GLuint buffer;

	Util::Array<Material*> materials;
};

}