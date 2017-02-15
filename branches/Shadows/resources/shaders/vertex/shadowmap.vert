/// Shadow Map ///

layout(location=0) in vec3 pos;

layout (std140, binding = 8) uniform shadowUniformBuffer
{
	mat4 LSM;
}shadowDerpBuffer;

void main() 
{
	gl_Position = shadowDerpBuffer.LSM * Model * vec4(pos, 1.0);	
}