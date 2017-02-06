#include <cstring>
#include "config.h"
#include "meshbuilder.h"
#include "meshfileformatstructs.h"
#include "meshresource.h"

namespace Render
{

MeshBuilder::MeshBuilder()
{
}

MeshBuilder::~MeshBuilder()
{
}

bool MeshBuilder::ExportMesh(std::shared_ptr<MeshResource> mesh, const char* filepath)
{
	FILE * file = fopen(filepath, "wb");
	if (file != nullptr)
	{
		//First a header!
		MeshFileHeader* header = new MeshFileHeader;
		header->magic = (uint)1337;//"MESH";
		header->numGroups = mesh->getNumPrimitiveGroups();
		header->numVertices = mesh->getNumVertices();
		header->vertexWidth = mesh->getVertexWidth();
		header->numIndices = mesh->getNumIndices();
		header->vertexComponentMask = mesh->getVertexComponentMask();

		//Write header
		fwrite(header, sizeof(MeshFileHeader), 1, file);

		//List all primitivegroups
		MeshFilePrimitiveGroup* g = new MeshFilePrimitiveGroup;
		for (uint i = 0; i < header->numGroups; ++i)
		{
			//Max limit of primitive group name is 256 letters
			_assert(mesh->getPrimitiveGroup(i).name.length() <= 256, "ERROR: A primitive groups name is longer than 256 letters! WHY WOULD YOU WANT THAT? :(");
			//TODO: Check if name is actually 256 letter long after this
			strcpy(g->name, mesh->getPrimitiveGroup(i).name.c_str());
			g->indexpointer = mesh->getPrimitiveGroup(i).indexOffset;
			g->numindices = mesh->getPrimitiveGroup(i).numIndices;
			fwrite(g, sizeof(MeshFilePrimitiveGroup), 1, file);
			
		}
		delete g;

		fwrite(mesh->getMesh(), header->numVertices * header->vertexWidth * sizeof(float), 1, file);
		fwrite(mesh->getIndices(), header->numIndices * sizeof(int), 1, file);

		delete header;

		fclose(file);
	}	
	return true;
}

}