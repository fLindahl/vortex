/// Shadow Map ///

layout(location = 0) in vec3 pos;

uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix * Model * vec4(pos, 1.0);
}