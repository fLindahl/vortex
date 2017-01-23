#pragma once

namespace Render
{
class MeshResource;

class MeshBuilder
{
public:
	MeshBuilder();
	~MeshBuilder();

	static bool ExportMesh(std::shared_ptr<MeshResource> mesh, const char* filepath);

private:

};
}