layout(location=0) in vec3 pos;
layout(location=2) in vec2 uv;

struct ParticleState
{
	vec4 pos;
	vec4 vel;
	vec4 rot;
	//float lifetime;
	vec4 accLife;
};

layout(std430, binding = 4) buffer source 
{
	ParticleState settings[]; // array of structures
};

//out vec3 FragmentPos;
//out vec3 Normal;
out vec2 TexCoords;
out vec4 inColor;

void main()
{
	vec3 particlePos = settings[gl_InstanceID].pos.xyz;
	vec3 camRight = vec3(View[0][0],View[1][0], View[2][0]);
	vec3 camUp = vec3(View[0][1],View[1][1], View[2][1]);
	vec3 vertexPos = particlePos + camRight*pos.x + camUp*pos.y;
	vec4 wPos = vec4(particlePos+(vertexPos*vec3(0.05,0.05,0.05)) , 1);
	
	// position in world space
	//FragmentPos = wPos.xyz;
	
	inColor = vec4(1.000, 0.451, 0.235, 1);
	
	// normal in world space
	//Normal = vec3(normalize(Model * vec4(normal, 0)));

	TexCoords = uv;

	gl_Position = ViewProjection * wPos;
	
	
}
