in vec3 FragmentPos;
//in vec3 Normal;
in vec2 TexCoords;
// For normalmapping
in mat3 NormalMatrix;
in vec4 FragPosLightSpace;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec3 normalColor;
layout(location = 2) out vec4 specularAndRoughness;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;
uniform sampler2D RoughnessMap;
uniform sampler2D ShadowMap;

struct PointLight
{
	int lightType;
	
	vec4 color;
	vec4 position;
	vec4 radiusAndPadding;
};

struct DirectionalLight 
{
	int lightType;
	
	vec4 color;
	vec4 direction;
};

struct SpotLight 
{
	int lightType;
	
    vec4 color;
	vec4 position;
	vec4 coneDirection;
	vec4 midPoint;
	float length;
	float fRadius;
	float angle;
};

struct VisibleIndex 
{
	int index;
};

layout (std140, binding = 24) uniform uniformBlock
{
	int pointLightCount;
	int spotLightCount;
	int directionalLightCount;
	int tileLights;
};

// Shader storage buffer objects
layout(std430, binding = 1) readonly buffer PointLightBuffer 
{
	PointLight data[];
} pointLightBuffer;

layout(std430, binding = 2) readonly buffer SpotLightBuffer 
{
	SpotLight data[];
} spotLightBuffer;

layout(std430, binding = 3) readonly buffer DirectionalLightBuffer 
{
	DirectionalLight data[];
} directionalLightBuffer;

layout(std430, binding = 4) readonly buffer VisiblePointLightIndicesBuffer 
{
	VisibleIndex data[];
} visiblePointLightIndicesBuffer;

layout(std430, binding = 5) readonly buffer VisibleSpotLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleSpotLightIndicesBuffer;

layout(std430, binding = 6) readonly buffer VisibleDirectionalLightIndicesBuffer 
{
	VisibleIndex data[];
} visibleDirectionalLightIndicesBuffer;

// parameters of the light and possible values
const vec3 u_lightAmbientIntensity = vec3(0.1f, 0.1f, 0.1f);

// Attenuate the point light intensity
float attenuate(vec3 lightDirection, float radius)
{
	vec3 l = lightDirection / radius;
    float atten = max(0.0, 1.0 - dot(l,l));

	return atten;
}

float DoSpotCone(SpotLight light, vec3 L)
{
    float minCos = cos(radians(light.angle));

	/// Lerps between minCos and 1
    float maxCos = mix(minCos, 1.0, 0.5);
    float cosAngle = dot(normalize(light.coneDirection.xyz), -L);
    
    return smoothstep(minCos, maxCos, cosAngle);
}
float DoAttenuation(SpotLight light, float distance)
{
    return 1.0f - smoothstep(light.length * 0.75f, light.length, distance);
}

// Assume the monitor is calibrated to the sRGB color space
const float screenGamma = 2.2;


void main()
{
	//uint tileLights = 512;
	
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(TILE_SIZE, TILE_SIZE);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	//Sample textures
	vec3 albedoDiffuseColor = texture(AlbedoMap,TexCoords).rgb;
	vec3 normal = texture(NormalMap, TexCoords).rgb;
	vec3 spec = texture(SpecularMap, TexCoords).rgb;
	float roughness = texture(RoughnessMap, TexCoords).r;

	float shininess = (roughness * 128) + 1;

	//Linearize colors
	vec3 N = NormalMatrix * ((normal*2.0f) - 1.0f);

	vec3 V = normalize(CameraPosition.xyz - FragmentPos.xyz);

	/// Loop for Point Lights
	uint offset = index * tileLights;
	for (uint i = 0; i < tileLights && visiblePointLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visiblePointLightIndicesBuffer.data[offset + i].index;
		PointLight light = pointLightBuffer.data[lightIndex];
		
		vec3 L = light.position.xyz - FragmentPos.xyz;

		float attenuation = attenuate(L, light.radiusAndPadding.x);

		L = normalize(L);
		float diffuse = max(dot(L, N), 0.0);
		float specular = 0.0f;

		//Hope this looks better with shadows...
		//if(diffuse > 0.0f)
		//{
			vec3 H = normalize(L + V);
			specular = pow(max(dot(H, N), 0.0), shininess);
		//}

		vec3 irradiance = (light.color.rgb * (albedoDiffuseColor.rgb * diffuse) + (vec3(specular) * spec)) * attenuation;
		
		color.rgb += irradiance;
	}

	/// Loop for SpotLights
	for (uint i = 0; i < tileLights && visibleSpotLightIndicesBuffer.data[offset + i].index != -1; i++)
	{		
		uint lightIndex = visibleSpotLightIndicesBuffer.data[offset + i].index;
		SpotLight light = spotLightBuffer.data[lightIndex];
	
		vec3 projcords = FragPosLightSpace.xyz / FragPosLightSpace.w;
		
		vec3 uvcords = 0.5f * projcords + 0.5f;
		
		float depth = texture(ShadowMap, uvcords.xy).r;
		float z = uvcords.z;
		
		float shadowfactor = 0.0f;
		float nordir = clamp(dot(normal.xyz, normalize(light.coneDirection.xyz)), 0,1);
		float bias = 0.0005f * tan(acos(nordir));
		bias = min(max(bias, 0.005f), 0.0005f);
		
		
		//if (projcords.z > 1.0 && projcords.z < 0.0) //outside of shadowmap and therefore it should not be shaded
			//shadowfactor = 0.0f;
		if(projcords.z < 1.0 && projcords.z > 0.0)
		{
			//PCF, sample pixles for smooth shadows. DO NOT INCREASE THE SAMPLE RATE: IT WILL HAVE MASSIVE PERFORMANCE IMPACT
			vec2 texelsize = 1.0 / textureSize(ShadowMap, 0);
			for (int x=-3; x<=3; ++x)
			{
				for(int y = -3; y <= 3; ++y)
				{
					float pcfDepth = texture(ShadowMap, uvcords.xy + vec2(x, y) * texelsize).r; 
					shadowfactor += z-bias > pcfDepth ? 1.0f : 0.0f;      
				}
			}
			shadowfactor /= 49.0f;
		}
		
		/// Light Direction
		vec3 L = light.position.xyz - FragmentPos.xyz;
		float distance = length(L);
		L = L / distance;
		
		float attenuation = DoAttenuation(light, distance);
		float spotIntensity = DoSpotCone(light, L);
		
		L = normalize(L);
		float diffuse = max(dot(L, N), 0.0);
		float specular = 0.0f;
		
		//Hope this looks better with shadows...
		//if(diffuse > 0.0f)
		//{
			vec3 H = normalize(L + V);
			specular = pow(max(dot(H, N), 0.0), shininess);		
		//}
		
		vec3 irradiance = (light.color.rgb * (albedoDiffuseColor.rgb * diffuse * (1.0f-shadowfactor)) + (vec3(specular) * spec) * (1.0f-shadowfactor)) * attenuation * spotIntensity;
		color.rgb += irradiance;
	}
	
	for (uint i = 0; i < tileLights && visibleDirectionalLightIndicesBuffer.data[offset + i].index != -1; i++)
	{
		uint lightIndex = visibleDirectionalLightIndicesBuffer.data[offset + i].index;
		DirectionalLight light = directionalLightBuffer.data[lightIndex];
		
		vec3 L = normalize(light.direction.xyz);

		float attenuation = 1.0f;
		
		float diffuse = max(dot(L, N), 0.0);
		float specular = 0.0f;

		//Hope this looks better with shadows...
		//if(diffuse > 0.0f)
		//{
			vec3 H = normalize(L + V);
			specular = pow(max(dot(H, N), 0.0), shininess);
		//}

		vec3 irradiance = (light.color.rgb * (albedoDiffuseColor.rgb * diffuse) + (vec3(specular) * spec)) * attenuation;
		
		color.rgb += irradiance;
	}
	
	color.rgb += albedoDiffuseColor.rgb * u_lightAmbientIntensity;

	fragColor = color;
	normalColor = N;
	specularAndRoughness.rgb = spec;
	specularAndRoughness.a = roughness;

}