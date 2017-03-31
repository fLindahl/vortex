#pragma once
#include "core/refcounted.h"

namespace Render
{
class MeshResource;

class MeshBuilder
{
public:
	MeshBuilder();
	~MeshBuilder();

	static bool ExportMesh(Ptr<MeshResource> mesh, const char* filepath);

private:

};
}