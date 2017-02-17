/// Shadow Map ///

layout(location = 0) in vec3 pos;

layout (std140, binding = 15) uniform shadowUniformBuffer
{
	mat4 LSM;
}shadowDerpBuffer;

uniform mat4 lightSpaceMatrix;
uniform mat4 lightProj;

void main() 
{
	gl_Position =  lightSpaceMatrix * Model * vec4(pos, 1.0);
}