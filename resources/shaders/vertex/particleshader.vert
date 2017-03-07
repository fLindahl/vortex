layout(location=0) in vec3 pos;
layout(location=2) in vec2 uv;

struct ParticleState
{
	vec4 pos;
	vec4 vel;
	vec4 rot;
	vec4 accLife;
	vec4 color;
	vec4 size;
	vec4 startSize;
	vec4 endSize;
};

layout(std140, binding = 3) uniform RenderBuffer 
{
	uint offset;
	//x = number of frames per row
	//y = total number of frames
	//w = if it is a spreedsheet or not 1.0 = spreedsheet 0.0 = no spreedsheet
	vec4 textureAnimation;
};  

layout(std430, binding = 4) buffer source 
{
	ParticleState settings[]; // array of structures
} pSettings;

layout(std430, binding=5) buffer StartSettings 
{
	ParticleState settings[]; // array of structures
} startSettings;

mat3 rotateZ(float rad) 
{
    float c = cos(rad);
    float s = sin(rad);
    return mat3(
        c, s, 0.0,
        -s, c, 0.0,
        0.0, 0.0, 1.0
    );
}


//out vec3 FragmentPos;
//out vec3 Normal;
out vec2 TexCoords;
out vec4 inColor;
out float render;

const float DT = 0.016;

void main()
{
	uint index = gl_InstanceID + offset;
	
	vec3 camRight = vec3(View[0][0],View[1][0], View[2][0]);
	vec3 camUp = vec3(View[0][1],View[1][1], View[2][1]);
	vec3 normVel =  normalize(pSettings.settings[index].vel.xyz);	
	float signX = sign(dot(camRight, normVel));
	vec3 scaleVec = pSettings.settings[index].size.xyz;
	mat3 scale = mat3(scaleVec.x, 0, 0,
					  0, scaleVec.y, 0,
					  0, 0, scaleVec.z);
	vec3 p = pos * rotateZ(signX * acos(dot(vec3(0,1,0),normVel))) * scale;
	vec3 particlePos = pSettings.settings[index].pos.xyz;
	vec3 vertexPos = camRight*p.x + camUp*p.y;
	
	vec4 wPos = vec4(particlePos+vertexPos , 1);
	
	// position in world space
	//FragmentPos = wPos.xyz;
	
	inColor = pSettings.settings[index].color;
	
	// normal in world space
	//Normal = vec3(normalize(Model * vec4(normal, 0)));

	if(textureAnimation.w >= 1.0)
	{
		float lerp = 1-(pSettings.settings[index].accLife.w / startSettings.settings[index].accLife.w);
		float indx = textureAnimation.y*lerp;
		
		vec2 offset = vec2((int(floor(indx))% int(textureAnimation.x))/textureAnimation.x, floor(indx/textureAnimation.x)/textureAnimation.x);
		
		TexCoords = (uv/textureAnimation.x)+offset;
	}
	else
	{
		TexCoords = uv;
	}
	

	gl_Position = ViewProjection * wPos;
	
	
}
