/// Shadow Map ///

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

out vec2 TexCoordsO;
out vec4 lightSpacePos;
out vec3 worldpos; //fragmentpos
out vec3 NormalO;

layout (std140, binding = 9) uniform shadowUniformBuffer
{
	mat4 DepthBias;
	mat4 lightM;
	mat4 lightV;
}shadowDerpBuffer;

void main() 
{
	gl_Position = ViewProjection * Model * vec4(pos, 1.0);
	lightSpacePos = ((shadowDerpBuffer.lightV * Projection) * shadowDerpBuffer.lightM) * vec4(pos, 1.0);
	worldpos = (Model * vec4(pos, 1.0)).xyz;
	NormalO = (Model * vec4(normal, 0.0)).xyz;
	TexCoordsO.x = uv.x;
	TexCoordsO.y = 1 - uv.y;
	
}