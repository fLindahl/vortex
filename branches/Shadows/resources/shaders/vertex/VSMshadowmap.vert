///MSM Shadow Map ///

layout(location = 0) in vec3 pos;


uniform mat4 lightSpaceMatrix;
uniform mat4 lightProj;

void main() 
{
	gl_Position =  lightSpaceMatrix * Model * vec4(pos, 1.0);
}