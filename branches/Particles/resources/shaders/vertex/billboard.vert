layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

out vec3 FragmentPos;
out vec3 Normal;
out vec2 TexCoords;

vec2 billboardsize = vec2(1,1);

void main()
{
	vec4 wPos = (Model * vec4(pos, 1.0f));
	
	// position in world space
	FragmentPos = wPos.xyz;	
	
	// normal in world space
	Normal = vec3(normalize(Model * vec4(normal, 0)));

	TexCoords = uv;

	gl_Position = ViewProjection * wPos;
	
}