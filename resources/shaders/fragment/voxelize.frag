in vec2 TexCoords;

uniform sampler2D AlbedoMap;

layout (rgba32f) uniform writeonly image3D VoxelTree;

void main()
{
	ivec3 voxel = ivec3(gl_FragCoord.xyz * vec3(256, 256, 256));
	
	imageStore(VoxelTree, voxel, vec4(1,1,1,1)));	
}