uniform vec3 lightPosition;
uniform vec3 cameraPosition;

layout(location=0) in vec4 pos;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;

out vec3 Normal;
out vec3 toLight;
out vec3 toCamera;
layout(location=0) out vec2 Texcoords;

void main()
{
	// position in world space
	vec4 wPos = Model * pos;

	// normal in world space
	Normal = vec3(normalize(Model * vec4(normal, 0)));

	// direction to light
	toLight = normalize(lightPosition - wPos.xyz);

	// direction to camera
	toCamera = normalize(cameraPosition - wPos.xyz);

	Texcoords = uv;
	Texcoords.y = 1 - Texcoords.y;

	gl_Position = Projection * View * wPos;
	
}