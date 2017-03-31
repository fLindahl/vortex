/**--------------------
Class : Render::GeometryProxy

Used for approximating geometry using OBBs as a preprocessing step. 
These can be used for accelerating intersection queries with the world.
ex. Parallax corrected cubemaps use these proxies to find an approximative intersection with the world to correct the intersectionpoint with the cubemap.
---------------*/

#pragma once
#include "core/refcounted.h"
#include "GL/glew.h"
#include <memory>
#include "foundation/math/matrix4.h"
#include "foundation/util/array.h"
//#include "cubemapnode.h"

namespace Render
{

class CubeMapNode;

class GeometryProxy : public Core::RefCounted
{
__DeclareClass(GeometryProxy);
public:
	
	GeometryProxy();
	GeometryProxy(Math::mat4 transform);
	~GeometryProxy();

	void Activate();
	void Deactivate();

	Math::mat4& Transform() { return this->transform; }

	const Util::Array<Ptr<CubeMapNode>>& GetConnectedCubemaps() const;
	void ConnectCubemap(Ptr<CubeMapNode> cubemap);
private:
	bool isActive;

	//Rotation, scale and position. Starts as a unit box. Scale to the size you need.
	Math::mat4 transform;

	//Contains pointers to each environmentprobe that is connected to this proxy
	Util::Array<Ptr<CubeMapNode>> connectedCubemaps;
};
}