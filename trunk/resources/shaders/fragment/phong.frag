in vec3 FragmentPos;
//in vec3 Normal;
in vec2 TexCoords;
// For normalmapping
in mat3 NormalMatrix;
in vec3 TangentFragmentPos;
in vec3 TangentViewPos;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 normalColor;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;

struct PointLight 
{
	vec4 color;
	vec4 position;
	vec4 radiusAndPadding;
};

struct VisibleIndex 
{
	int index;
};

// Shader storage buffer objects
layout(std430, binding = 1) readonly buffer LightBuffer 
{
	PointLight data[];
} lightBuffer;

layout(std430, binding = 2) readonly buffer VisibleLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleLightIndicesBuffer;


// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.1f, 0.1f, 0.1f);

// parameters of the material and possible values
uniform vec4 AmbientReflectance; 
uniform vec4 DiffuseReflectance;
uniform vec4 SpecularReflectance;
uniform float Shininess;


// Attenuate the point light intensity
float attenuate(vec3 lightDirection, float radius) 
{
	vec3 l = lightDirection / radius;
    float atten = max(0.0, 1.0 - dot(l,l));
		
	return atten;
}

void main()
{
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	
	//Sample textures
	vec3 albedoDiffuseColor = texture(AlbedoMap,TexCoords).rgb;
	vec3 normal = texture(NormalMap, TexCoords).rgb;
	//Linearize colors
	vec3 N = NormalMatrix * ((normal*2.0f) - 1.0f);
	
	vec3 V = normalize(CameraPosition.xyz - FragmentPos.xyz);
	
	uint offset = index * 1024;
	for (uint i = 0; i < 1024 && visibleLightIndicesBuffer.data[offset + i].index != -1; i++) 
	{
		uint lightIndex = visibleLightIndicesBuffer.data[offset + i].index;
		PointLight light = lightBuffer.data[lightIndex];

		vec3 L = light.position.xyz - FragmentPos.xyz;
		
		float attenuation = attenuate(L, light.radiusAndPadding.x);

		L = normalize(L);
		
		vec3 H = normalize(L + V);

		// Calculate the diffuse and specular components of the irradiance, then irradiance, and accumulate onto color
		float diffuse = max(dot(L, N), 0.0);
		float specular = pow(max(dot(N, H), 0.0), Shininess) * (diffuse <= 0 ? 0 : 1);

		
		
		vec3 irradiance = light.color.rgb * (albedoDiffuseColor.rgb * diffuse + vec3(specular)) * attenuation;
		color.rgb += irradiance;
	}
	
	color.rgb += albedoDiffuseColor.rgb * u_lightAmbientIntensity;
	
	fragColor = color;
	normalColor = N;

}