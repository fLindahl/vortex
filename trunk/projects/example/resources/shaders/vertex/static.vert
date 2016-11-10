uniform vec4 lightPosition;
uniform vec4 cameraPosition;

layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

out vec3 Normal;
out vec3 toLight;
out vec3 toCamera;
layout(location=0) out vec2 Texcoords;

void main()
{
	// position in world space
	vec4 wPos = Model * vec4(pos, 1.0f);

	// normal in world space
	Normal = vec3(normalize(Model * vec4(normal, 0)));

	// direction to light
	toLight = normalize(lightPosition.xyz - wPos.xyz);

	// direction to camera
	toCamera = normalize(cameraPosition.xyz - wPos.xyz);

	Texcoords = uv;

	gl_Position = Projection * View * wPos;
	
}