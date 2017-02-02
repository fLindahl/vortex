layout(location=0) in vec3 pos;
layout(location=2) in vec2 uv;

struct ParticleState
{
	vec4 pos;
	vec4 vel;
	vec4 rot;
	vec4 accLife;
	vec4 color;
};

layout(std140, binding = 3) uniform RenderBuffer 
{
	vec4 startSize;
	vec4 endSize;
	uint offset;
};  

layout(std430, binding = 4) buffer source 
{
	ParticleState settings[]; // array of structures
} pSettings;

layout(std430, binding=5) buffer StartSettings 
{
	ParticleState settings[]; // array of structures
} startSettings;

//out vec3 FragmentPos;
//out vec3 Normal;
out vec2 TexCoords;
out vec4 inColor;
out float render;

const float DT = 0.016;

void main()
{
	uint index = gl_InstanceID + offset;
	float lerp = pSettings.settings[index].accLife[3]/startSettings.settings[index].accLife[3];
	vec3 particlePos = pSettings.settings[index].pos.xyz;
	vec3 scalepos = pos * mix(endSize,startSize, lerp).xyz;
	vec3 camRight = vec3(View[0][0],View[1][0], View[2][0]);
	vec3 camUp = vec3(View[0][1],View[1][1], View[2][1]);
	vec3 vertexPos = particlePos + camRight*scalepos.x + camUp*scalepos.y;
	vec4 wPos = vec4(particlePos+vertexPos , 1);
	
	// position in world space
	//FragmentPos = wPos.xyz;
	
	//Check if fragment should be discarded. if it should be rendered send 1.0 else 0.0
	if(startSettings.settings[index].vel.w == 1.0)
	{
		render = 1.0;
		
	}
	else
	{
		render = 0.0;
	}
		
	inColor = pSettings.settings[index].color;
	
	// normal in world space
	//Normal = vec3(normalize(Model * vec4(normal, 0)));

	TexCoords = uv;

	gl_Position = ViewProjection * wPos;
	
	
}
