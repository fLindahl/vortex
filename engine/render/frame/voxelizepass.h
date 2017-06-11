/*
	Compute pass that generates a reflection buffer that we can multiply with our specular buffer
*/

#pragma once
#include "framepass.h"

namespace Render
{
class VoxelizePass : public DrawPass
{
	__DeclareClass(VoxelizePass);

public:	
	VoxelizePass();
	~VoxelizePass();

    void Setup();

    void Execute();

	void UpdateResolution();

	///Sets the voxel tree size if it's a power of 2.
	bool SetVoxelTreeSize(uint size);
	uint GetVoxelTreeSize() const;

private:
	//Uniform Buffer Object
	GLuint ubo[1];

	uint voxelTreeSize;

	GLuint voxelBuffer;
	
	GLuint queries[1];
};

}