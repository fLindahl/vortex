/// Shadow Map ///

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;

//out vec2 oUV;
//out vec4 Shadowcoord;

layout (std140, binding = 9) uniform shadowUniformBuffer
{
	mat4 DepthBias;
	mat4 lightSpaceMatrix;
}shadowDerpBuffer;

void main() 
{
	gl_Position = ViewProjection * Model * vec4(Position, 1.0);
	//Shadowcoord = shadowDerpBuffer.DepthBias * gl_Position;
	
	//oUV = UV;
}